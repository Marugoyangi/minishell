/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 19:23:09 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/13 00:31:37 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int exec_redirection(t_node *node, t_arg *arg);
int exec_output(t_node *node, t_arg *arg);
int exec_append(t_node *node, t_arg *arg);
int exec_input(t_node *node, t_arg *arg);
int exec_heredoc(t_node *node, t_arg *arg);

int	exec_redirection(t_node *node, t_arg *arg)
{
	if (!ft_strcmp(node->data, ">>"))
		return (exec_append(node, arg));
	else if (!ft_strcmp(node->data, ">"))
		return (exec_output(node, arg));
	else if (!ft_strcmp(node->data, "<<"))
		return (exec_heredoc(node, arg));
	else if (!ft_strcmp(node->data, "<"))
		return (exec_input(node, arg));
	return (1);
}

int	exec_output(t_node *node, t_arg *arg)
{
	int		fd;
	int		status;
	pid_t	pid;

	fd = open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (!node->left)
		node = node->right;
	else
		node = node->left;
	arg->fork_sign++;
	pid = fork();
	if (!pid)
	{
		dup2(fd, 1);
		close(fd);
		start_exec(node, arg);
	}
	else
	{
		waitpid(pid, &status, 0);
		arg->fork_sign--;
	}
	return (status);
}

int	exec_append(t_node *node, t_arg *arg)
{
	int		fd;
	int		status;
	pid_t	pid;

	fd = open(node->argv[0], O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (!node->left)
		node = node->right;
	else
		node = node->left;
	arg->fork_sign++;
	pid = fork();
	if (!pid)
	{
		dup2(fd, 1);
		close(fd);
		start_exec(node, arg);
	}
	else
	{
		waitpid(pid, &status, 0);
		arg->fork_sign--;
	}
	return (status);
}

int	exec_input(t_node *node, t_arg *arg)
{
	int		fd;
	int		status;
	pid_t	pid;

	fd = open(node->argv[0], O_RDONLY);
	if (!node->left)
		node = node->right;
	else
		node = node->left;
	arg->fork_sign++;
	pid = fork();
	if (!pid)
	{
		dup2(fd, 1);
		close(fd);
		start_exec(node, arg);
	}
	else
	{
		waitpid(pid, &status, 0);
		arg->fork_sign--;
	}
	return (status);
}

int	exec_heredoc(t_node *node, t_arg *arg)
{
	int		fd;
	int		status;
	pid_t	pid;

	fd = open(node->filename, O_RDONLY);
	if (!node->left)
		node = node->right;
	else
		node = node->left;
	arg->fork_sign++;
	pid = fork();
	if (!pid)
	{
		dup2(fd, 1);
		close(fd);
		start_exec(node, arg);
	}
	else
	{
		waitpid(pid, &status, 0);
		unlink(node->filename);
		arg->fork_sign--;
	}
	return (status);
}
