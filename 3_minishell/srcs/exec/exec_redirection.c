/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 19:23:09 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/13 06:33:09 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int exec_redirection(t_node *node, t_arg *arg);
int exec_output(t_node *node, t_arg *arg);
int exec_append(t_node *node, t_arg *arg);
int exec_input(t_node *node, t_arg *arg);
int exec_heredoc(t_node *node, t_arg *arg);
void	exec_redirection_child(t_node *node, t_arg *arg, int fd, int io);
t_node	*get_redirection_node(t_node *node);
// int	single_redirection(t_node *node, t_arg *arg);


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

void	exec_redirection_child(t_node *node, t_arg *arg, int fd, int io)
{
	dup2(fd, io);
	close(fd);
	start_exec(node, arg);
}

t_node	*get_redirection_node(t_node *node)
{
	if (!node->left && !node->right)
		return (0);
	else if (!node->left)
		node = node->right;
	else
		node = node->left;
	return (node);
}

int	error_redirection(t_node *node, int fd)
{
	if (fd < 0)
	{
		printf("minishell: %s: No such file or directory\n", node->argv[0]);
		return (1);
	}
	if (!node)
	{
		printf("minishell: syntax error, redirection need two full pairs of fd\n");
		return (1);
	}
	return (0);
}

int	exec_output(t_node *node, t_arg *arg)
{
	int		fd;
	int		status;
	pid_t	pid;

	fd = open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	node = get_redirection_node(node);
	if (error_redirection(node, fd))
		return (0);
	if (!node)
		return (0);
	if (!arg->fork_sign)
	{
		arg->fork_sign++;
		pid = fork();
		if (!pid)
			exec_redirection_child(node, arg, fd, 1);
		else
		{
			waitpid(pid, &status, 0);
			arg->fork_sign--;
			return (status);
		}
	}
	else
		exec_redirection_child(node, arg, fd, 1);
	return (1);	// 여기 걸리면 오류인것
}

int	exec_append(t_node *node, t_arg *arg)
{
	int		fd;
	int		status;
	pid_t	pid;

	fd = open(node->argv[0], O_WRONLY | O_CREAT | O_APPEND, 0666);
	node = get_redirection_node(node);
	if (error_redirection(node, fd))
		return (0);
	if (!arg->fork_sign)
	{
		arg->fork_sign++;
		pid = fork();
		if (!pid)
			exec_redirection_child(node, arg, fd, 1);
		else
		{
			waitpid(pid, &status, 0);
			arg->fork_sign--;
			return (status);
		}
	}
	else
		exec_redirection_child(node, arg, fd, 1);
	return (1);	// 여기 걸리면 오류인것
}

int	exec_input(t_node *node, t_arg *arg)
{
	int		fd;
	int		status;
	pid_t	pid;

	fd = open(node->argv[0], O_RDONLY);
	node = get_redirection_node(node);
	if (error_redirection(node, fd))
		return (0);
	if (!arg->fork_sign)
	{
		arg->fork_sign++;
		pid = fork();
		if (!pid)
			exec_redirection_child(node, arg, fd, 0);
		else
		{
			waitpid(pid, &status, 0);
			arg->fork_sign--;
			return (status);
		}
	}
	else
		exec_redirection_child(node, arg, fd, 0);
	return (1);	// 여기 걸리면 오류인것
}

int	exec_heredoc(t_node *node, t_arg *arg)
{
	int		fd;
	int		status;
	pid_t	pid;

	fd = open(node->filename, O_RDONLY);
	unlink(node->filename);
	node = get_redirection_node(node);
	if (error_redirection(node, fd))
		return (0);
	if (!arg->fork_sign)
	{
		arg->fork_sign++;
		pid = fork();
		if (!pid)
			exec_redirection_child(node, arg, fd, 0);
		else
		{
			waitpid(pid, &status, 0);
			arg->fork_sign--;
			return (status);
		}
	}
	else
		exec_redirection_child(node, arg, fd, 0);
	return (1);	// 여기 걸리면 오류인것
}
