/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungwok <seungwok@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 19:23:09 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/16 17:03:11 by seungwok         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		exec_redirection(t_node *node, t_arg *arg);
int		check_built_in_redirection(t_node *node);
void	exec_redirection_child(t_redirection *node, t_arg *arg, int *fd);
void	exec_redirection_parent(t_arg *arg, pid_t pid, int *status);

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
		return(check_built_in(redirection_node.exec_node, arg));
	arg->fork_sign++;
	pid = fork();
	if (!pid)
		exec_redirection_child(&redirection_node, arg, fd);
	else
		exec_redirection_parent(arg, pid, &status);
	return (WEXITSTATUS(status));
}

void	exec_redirection_child(t_redirection *node, t_arg *arg, int *fd)
{
	if (node->input_node)
	{
		dup2(fd[0], 0);
		close(fd[0]);
	}
	if (node->output_node)
	{
		dup2(fd[1], 1);
		close(fd[1]);
	}
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

int	check_built_in_redirection(t_node *node)
{
	if (!ft_strcmp(node->data, "echo")
		|| !ft_strcmp(node->data, "cd")
		|| !ft_strcmp(node->data, "pwd")
		|| !ft_strcmp(node->data, "exit")
		|| !ft_strcmp(node->data, "export")
		|| !ft_strcmp(node->data, "unset")
		|| !ft_strcmp(node->data, "env"))
		return (0);
	return (1);
}