/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 02:15:38 by woopinbell        #+#    #+#             */
/*   Updated: 2023/12/17 02:37:17 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		exec_redirection(t_node *node, t_arg *arg);
int		check_built_in_redirection(t_node *node);
int		built_in_redirection_fd(t_redirection *node, t_arg *arg, int *fd);
void	exec_redirection_child(t_redirection *node, t_arg *arg, int *fd);
void	exec_redirection_parent(t_arg *arg, pid_t pid, int *status, int *fd);

int	exec_redirection(t_node *node, t_arg *arg)
{
	t_redirection	redirection_node;
	int				fd[2];
	int				status;
	pid_t			pid;

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
	arg->fork_sign++;
	pid = fork();
	if (!pid)
		exec_redirection_child(&redirection_node, arg, fd);
	else
	{
		if (redirection_node.input_node)
			close(fd[0]);
		if (redirection_node.output_node)
			close(fd[1]);
		exec_redirection_parent(arg, pid, &status, fd);
	}
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

void	exec_redirection_parent(t_arg *arg, pid_t pid, int *status, int *fd)
{
	fd = fd;
	waitpid(pid, status, 0);
	arg->fork_sign--;
}
