/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 02:02:43 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:13:59 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_heredoc_prefix(t_arg *arg, int *i, int *row)
{
	char	*tmp;
	int		k;

	k = 0;
	tmp = NULL;
	tmp = modified_strjoin("\001\033[", ft_itoa(*row), 2);
	tmp = modified_strjoin(tmp, "A\002", 1);
	while (k < *i + 1)
	{
		tmp = modified_strjoin(tmp, ft_strdup(get_ps(arg, 2)), 0);
		k++;
	}
	return (tmp);
}

void	init_file_heredoc_iter(int *i, int *row, char *line, int fd)
{
	*i = 0;
	*row = -1;
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free (line);
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
		init_file_heredoc_iter(i, row, line, fd);
	}
	close(fd);
}

void	set_heredoc(t_node *node, t_arg *arg, int *row, int *i)
{
	if (!node)
		return ;
	if (node->data && node->type == L_REDIRECTION && \
	!ft_strcmp(node->data, "<<") && node->argv && node->argv[0])
		init_file_for_heredoc(node, row, i, arg);
	set_heredoc(node->right, arg, row, i);
}

void	get_heredoc(t_arg *arg)
{
	int	pid;
	int	xy[2];
	int	status;

	heredoc_variables_init(&status, &pid, xy);
	get_heredoc_filename(arg->ast_head, &xy[0], arg);
	xy[0] = 0;
	pid = fork();
	if (!pid)
	{
		signal(SIGINT, sig_handler_waiting);
		signal(SIGQUIT, SIG_IGN);
		set_heredoc(arg->ast_head, arg, &xy[1], &xy[0]);
		exit (0);
	}
	signal(SIGINT, sig_handler_heredoc);
	signal(SIGQUIT, sig_handler_heredoc);
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) != 0)
		arg->error->code = 1;
	terminal_interactive(arg);
}
