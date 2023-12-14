/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_set_start.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:25:08 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/15 00:42:27 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		start_exec(t_node *node, t_arg *arg);

void	get_heredoc(t_arg *arg)
{
	int	pid;
	int	count;
	int	i;
	int	status;

	status = 0;
	pid = 0;
	count = 0;
	i = 1;
	get_heredoc_filename(arg->ast_head, &i);
	i = 1;
	pid = fork();
	if (!pid)
	{
		signal(SIGINT, sig_handler_heredoc);
		signal(SIGQUIT, sig_handler_heredoc);
		set_heredoc(arg->ast_head, arg, &count, &i);
		exit (0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) == 1)
		{
			arg->last_exit_status = WEXITSTATUS(status);
			arg->error->code = WEXITSTATUS(status);
		}
		terminal_interactive(arg);
	}
}

void	set_exec(t_arg *arg)
{
	arg->fork_sign = 0;
	arg->last_exit_status = start_exec(arg->ast_head, arg);
}

char	*set_heredoc_filename(int *i)
{
	int		fd;
	char	*filename;

	while (1)
	{
		filename = modified_strjoin(".", ft_itoa(*i), 2);
		fd = open(filename, O_RDONLY);	// open 성공 = filename 파일 존재
		if (fd == -1)
		{
			(*i)++;
			return (filename);
		}
		close(fd);
		free(filename);
		(*i)++;
	}
}

void	init_file_for_heredoc(t_node *node, int *count, int *i)
{
	int		fd;
	char	*line;
	char	*tmp;
	char	*tmp_cursor;
	int		k;

	k = 0;
	*count = *count + 1;
	fd = open(node->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	while (1)
	{
		k = 0;
		tmp_cursor = modified_strjoin("\033[", ft_itoa(*i - *count), 2);
		tmp_cursor = modified_strjoin(tmp_cursor, "A\033[K> ", 1);
		tmp = ft_strdup(tmp_cursor);
		while (k < *i)
		{
			tmp = modified_strjoin(tmp, ft_strdup("> "), 0);
			k++;
		}
		line = readline(tmp);
		if (!ft_strcmp(line, node->argv[0]))
		{
			free(line);
			break ;
		}
		else if (line)
		{
			*i = 0;
			*count = 0;
		}
		else if (!line)
		{
			if (*i == *count)
				*i = *i + 1;
			else
			{
				*i = 0;
				*count -= 1;
			}
			close (fd);
			return ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
}

// heredoc 찾아서 입력값 받은 후 파일에 저장하기
void	set_heredoc(t_node *node, t_arg *arg, int *count, int *i)
{
	if (!node)
		return ;
	if (node->type == L_REDIRECTION && !ft_strcmp(node->data, "<<"))
		init_file_for_heredoc(node, count, i);
	set_heredoc(node->left, arg, count, i);
	set_heredoc(node->right, arg, count, i);
	exit (0);
}

int	start_exec(t_node *node, t_arg *arg)
{
	if (!node)
		return (0);
	if (node->type == L_SUBSHELL)
		return (exec_subshell(node, arg));
	else if (node->type == L_LOGICAL_OPERATOR)
		return (exec_logical_operator(node, arg));
	else if (node->type == L_PIPELINE)
		return (exec_pipeline(node, arg));
	else if (node->type == L_REDIRECTION)
		return (exec_redirection(node, arg));
	else if (node->type == L_SIMPLE_COMMAND)
		return (exec_command(node, arg));
	else
		return (1);	// 노드가 비어있지않은데 위 분기에 없다면 오류.
}
