/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 02:02:43 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/16 04:13:35 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*find_heredoc_prefix(t_arg *arg, int *i, int *row)
{
	char	*tmp;
	int		k;

	k = 0;
	tmp = NULL;
	tmp = modified_strjoin("\033[", ft_itoa(*row), 2);
	tmp = modified_strjoin(tmp, "A\033[K", 1);
	while (k < *i + 1)
	{
		tmp = modified_strjoin(tmp, ft_strdup(find_env(arg->envp_head, "PS2")), 0);
		k++;
	}
	return (tmp);
}

void	init_file_for_heredoc(t_node *node, int *row, int *i, t_arg *arg)
{
	int		fd;
	char	*line;
	char	*tmp;

	fd = open(node->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	while (1)
	{
		tmp = find_heredoc_prefix(arg, i, row);
		line = readline(tmp);
		free (tmp);
		if (node->argv && !ft_strcmp(line, node->argv[0]))
		{
			free(line);
			break ;
		}
		else if (!line)
		{
			*i += 1;
			*row = 0;
			break ;
		}
		else
		{
			*i = 0;
			*row = -1;
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
			free (line);
		}
	}
	close(fd);
}

void	set_heredoc(t_node *node, t_arg *arg, int *row, int *i)
{
	if (!node)
		return ;
	if (node->type == L_REDIRECTION && !ft_strcmp(node->data, "<<"))
		init_file_for_heredoc(node, row, i, arg);
	if (arg->error->code == 2)
		set_heredoc(node->right, arg, row, i);
	else
	{
		set_heredoc(node->left, arg, row, i);
		set_heredoc(node->right, arg, row, i);
	}
}

void	get_heredoc_filename(t_node *root, int *i, t_arg *arg)
{
	t_node	*node;

	if (!root)
		return ;
	node = root;
	if (node->type == L_REDIRECTION)
	{
		if (node->data && !ft_strcmp(node->data, "<<"))
			node->filename = set_heredoc_filename(i);
	}
	if (arg->error->code == 2)
		get_heredoc_filename(node->right, i, arg);
	else
	{
		get_heredoc_filename(node->left, i, arg);
		get_heredoc_filename(node->right, i, arg);
	}
}

void	get_heredoc(t_arg *arg)
{
	int	pid;
	int	row;
	int	i;
	int	status;

	status = 0;
	pid = 0;
	row = -1;
	i = 1;
	get_heredoc_filename(arg->ast_head, &i, arg);
	i = 0;
	pid = fork();
	if (!pid)
	{
		signal(SIGINT, sig_handler_heredoc);
		signal(SIGQUIT, SIG_IGN);
		set_heredoc(arg->ast_head, arg, &row, &i);
		exit (0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) == 1)
			arg->last_exit_status = WEXITSTATUS(status);
		terminal_interactive(arg);
	}
}