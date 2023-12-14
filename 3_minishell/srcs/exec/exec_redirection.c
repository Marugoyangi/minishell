/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 19:23:09 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/15 01:36:00 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int exec_redirection(t_node *node, t_arg *arg);
t_node *get_input(t_node *node, int *fd);
t_node *get_output(t_node *node, int *fd);

// int exec_output(t_node *node, t_arg *arg);
// int exec_append(t_node *node, t_arg *arg);
// int exec_input(t_node *node, t_arg *arg);
// int exec_heredoc(t_node *node, t_arg *arg);
// void	exec_redirection_child(t_node *node, t_arg *arg, int fd, int io);
// t_node	*get_redirection_node(t_node *node);

t_node *get_input(t_node *node, int *fd)
{
	t_node	*input_node;
	
	fd[0] = 0;
	input_node = 0;
	while(node)
	{
		if (!ft_strcmp(node->data, "<") || !ft_strcmp(node->data, "<<"))
		{
			if (!ft_strcmp(node->data, "<"))
				fd[0] = open(node->argv[0], O_RDONLY);
			else if (!ft_strcmp(node->data, "<<"))
				fd[0] = open(node->filename, O_RDONLY);
			if (fd[0] == -1)
				return (0);
			close(fd[0]);
			input_node = node;
		}
		node = node->left;
	}
	return (input_node);
}

t_node *get_output(t_node *node, int *fd)
{
	t_node	*output_node;

	output_node = 0;
	fd[1] = 0;
	while(node)
	{
		if (!ft_strcmp(node->data, ">") || !ft_strcmp(node->data, ">>"))
		{
			fd[1] = open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
			output_node = node;
			close (fd[1]);
		}
		node = node->left;
	}
	return (output_node);
}

int	exec_redirection(t_node *node, t_arg *arg)
{
	t_node	*exec_node;
	t_node	*input_node;
	t_node	*output_node;
	int		fd[2];
	int		status;
	pid_t	pid;

	exec_node = node->right;
	input_node = get_input(node, fd);
	output_node = get_output(node, fd);
	if (!input_node && fd[0] == -1)
	{
		printf("minishell: No such file or directory: %s\n", node->argv[0]);
		return (1);
	}
	if (input_node)
	{
		if (!ft_strcmp(input_node->data, "<"))
			fd[0] = open(node->argv[0], O_RDONLY);
		else
			fd[0] = open(node->filename, O_RDONLY);
	}
	if (output_node)
	{
		if (!ft_strcmp(output_node->data, ">"))
			fd[1] = open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else
			fd[1] = open(node->argv[0], O_WRONLY | O_CREAT | O_APPEND, 0666);
	}
	arg->fork_sign++;
	pid = fork();
	if (!pid)
	{
		if (input_node)
		{
			dup2(fd[0], 0);
			close(fd[0]);
		}
		if (output_node)
		{
			dup2(fd[1], 1);
			close(fd[1]);
		}
		if (start_exec(exec_node, arg))
			exit (1);
		else
			exit (0);
	}
	else
	{
		waitpid(pid, &status, 0);
		arg->fork_sign--;
		return (pid);
	}
}

// void	exec_redirection_child(t_node *node, t_arg *arg, int fd, int io)
// {
// 	dup2(fd, io);
// 	close(fd);
// 	if (start_exec(node->right, arg))
// 		exit (1);
// 	else
// 		exit (0);
// }

// t_node	*get_redirection_node(t_node *node)
// {
// 	if (!node->left && !node->right)
// 		return (0);
// 	else if (!node->left)
// 		node = node->right;
// 	else
// 		node = node->left;
// 	return (node);
// }

// int	error_redirection(t_node *node, int fd)
// {
// 	if (fd < 0)
// 	{
// 		printf("minishell: %s: No such file or directory\n", node->argv[0]);
// 		return (1);
// 	}
// 	if (!node)
// 	{
// 		printf("minishell: syntax error, redirection need two full pairs of fd\n");
// 		return (1);
// 	}
// 	return (0);
// }

// int	exec_output(t_node *node, t_arg *arg)
// {
// 	int		fd;
// 	int		status;
// 	pid_t	pid;

// 	fd = open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
// 	node = get_redirection_node(node);
// 	if (error_redirection(node, fd))
// 		return (0);
// 	if (!node)
// 		return (0);
// 	if (!arg->fork_sign)
// 	{
// 		arg->fork_sign++;
// 		pid = fork();
// 		if (!pid)
// 			exec_redirection_child(node, arg, fd, 1);
// 		else
// 		{
// 			waitpid(pid, &status, 0);
// 			arg->fork_sign--;
// 			return (status);
// 		}
// 	}
// 	else
// 		exec_redirection_child(node, arg, fd, 1);
// 	return (1);	// 여기 걸리면 오류인것
// }

// int	exec_append(t_node *node, t_arg *arg)
// {
// 	int		fd;
// 	int		status;
// 	pid_t	pid;

// 	fd = open(node->argv[0], O_WRONLY | O_CREAT | O_APPEND, 0666);
// 	node = get_redirection_node(node);
// 	if (error_redirection(node, fd))
// 		return (0);
// 	if (!arg->fork_sign)
// 	{
// 		arg->fork_sign++;
// 		pid = fork();
// 		if (!pid)
// 			exec_redirection_child(node, arg, fd, 1);
// 		else
// 		{
// 			waitpid(pid, &status, 0);
// 			arg->fork_sign--;
// 			return (status);
// 		}
// 	}
// 	else
// 		exec_redirection_child(node, arg, fd, 1);
// 	return (1);	// 여기 걸리면 오류인것
// }

// int	exec_input(t_node *node, t_arg *arg)
// {
// 	int		fd;
// 	int		status;
// 	pid_t	pid;

// 	fd = open(node->argv[0], O_RDONLY);
// 	node = get_redirection_node(node);
// 	if (error_redirection(node, fd))
// 		return (0);
// 	if (!arg->fork_sign)
// 	{
// 		arg->fork_sign++;
// 		pid = fork();
// 		if (!pid)
// 			exec_redirection_child(node, arg, fd, 0);
// 		else
// 		{
// 			waitpid(pid, &status, 0);
// 			arg->fork_sign--;
// 			return (status);
// 		}
// 	}
// 	else
// 		exec_redirection_child(node, arg, fd, 0);
// 	return (1);	// 여기 걸리면 오류인것
// }

// int	exec_heredoc(t_node *node, t_arg *arg)
// {
// 	int		fd;
// 	int		status;
// 	pid_t	pid;

// 	fd = open(node->filename, O_RDONLY);
// 	unlink(node->filename);
// 	node = get_redirection_node(node);
// 	// if (error_redirection(node, fd))
// 	// 	return (0);
// 	if (!arg->fork_sign)
// 	{
// 		arg->fork_sign++;
// 		pid = fork();
// 		if (!pid)
// 			exec_redirection_child(node, arg, fd, 0);
// 		else
// 		{
// 			waitpid(pid, &status, 0);
// 			arg->fork_sign--;
// 			return (status);
// 		}
// 	}
// 	else
// 		exec_redirection_child(node, arg, fd, 0);
// 	return (1);	// 여기 걸리면 오류인것
// }
