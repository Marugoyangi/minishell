/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection_built_in.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungwok <seungwok@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 02:15:38 by woopinbell        #+#    #+#             */
/*   Updated: 2023/12/21 17:55:21 by seungwok         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_with_close(int fd1, int fd2)
{
	dup2(fd1, fd2);
	close(fd1);
}

int	check_built_in_redirection(t_node *node)
{
	if (!node || !node->data)
		return (0);
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

int	built_in_redirection_fd(t_redirection *node, t_arg *arg, int *fd)
{
	int		fd_origin[2];
	int		return_value;

	fd_origin[0] = dup(0);
	fd_origin[1] = dup(1);
	if (node->input_node)
		dup_with_close(fd[0], 0);
	if (node->output_node)
		dup_with_close(fd[1], 1);
	return_value = built_in_redirection_fd2(node, arg);
	if (node->input_node)
	{
		close(0);
		dup_with_close(fd_origin[0], 0);
	}
	else
		close(fd_origin[0]);
	if (node->output_node)
	{
		close(1);
		dup_with_close(fd_origin[1], 1);
	}
	else
		close(fd_origin[1]);
	return (return_value);
}

int	built_in_redirection_fd2(t_redirection *node, t_arg *arg)
{
	if (!node->exec_node)
		return (-1);
	if (!ft_strcmp(node->exec_node->data, "echo"))
		return (built_in_echo(node->exec_node->argv));
	else if (!ft_strcmp(node->exec_node->data, "cd"))
		return (built_in_cd(node->exec_node, arg, node->exec_node->argv));
	else if (!ft_strcmp(node->exec_node->data, "pwd"))
		return (built_in_pwd(arg));
	else if (!ft_strcmp(node->exec_node->data, "exit"))
		return (built_in_exit(node->exec_node, arg));
	else if (!ft_strcmp(node->exec_node->data, "export"))
		return (built_in_export(node->exec_node, arg->envp_head));
	else if (!ft_strcmp(node->exec_node->data, "unset"))
		return (built_in_unset(node->exec_node, arg));
	else if (!ft_strcmp(node->exec_node->data, "env"))
		return (built_in_env(node->exec_node, arg->envp_head));
	return (-1);
}
