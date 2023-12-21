/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:51:54 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/19 17:37:33 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_redirection_fork(t_redirection *node, \
							t_arg *arg, int *fd, int *status)
{
	pid_t	pid;

	pid = 0;
	arg->fork_sign++;
	pid = fork();
	if (pid == -1)
	{
		*status = fail_to_fork(arg, 0);
		return ;
	}
	if (!pid)
		exec_redirection_child(node, arg, fd);
	else
	{
		if (node->input_node)
			close(fd[0]);
		if (node->output_node)
			close(fd[1]);
		exec_redirection_parent(arg, pid, status);
	}
}

int	exec_redirection(t_node *node, t_arg *arg)
{
	t_redirection	redirection_node;
	int				fd[2];
	int				status;

	status = 0;
	redirection_node.exec_node = node->right;
	redirection_node.input_node = get_input_node(node, fd);
	redirection_node.output_node = get_output_node(node, fd);
	if (fd[0] == -1)
		return (error_input_fd(redirection_node.input_node));
	if (redirection_node.input_node)
		fd[0] = get_input_fd(redirection_node.input_node);
	if (redirection_node.output_node)
		fd[1] = get_output_fd(redirection_node.output_node);
	if (!check_built_in_redirection(redirection_node.exec_node))
		return (built_in_redirection_fd(&redirection_node, arg, fd));
	exec_redirection_fork(&redirection_node, arg, fd, &status);
	return (WEXITSTATUS(status));
}

void	exec_redirection_child(t_redirection *node, t_arg *arg, int *fd)
{
	if (node->input_node)
		dup_with_close(fd[0], 0);
	if (node->output_node)
		dup_with_close(fd[1], 1);
	if (start_exec(node->exec_node, arg))
		exit (1);
	else
		exit (0);
}

void	exec_redirection_parent(t_arg *arg, pid_t pid, int *status)
{
	waitpid(pid, status, 0);
	arg->fork_sign--;
}
