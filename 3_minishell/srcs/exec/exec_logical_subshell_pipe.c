/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_logical_subshell_pipe.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 18:54:16 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/13 18:50:13 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		exec_logical_operator(t_node *node, t_arg *arg);
int		exec_subshell(t_node *node, t_arg *arg);
int		exec_pipeline(t_node *node, t_arg *arg);
int		exec_parent(t_arg *arg, int *pid, int *pipe);
void	exec_pipe_child1(t_node *node, t_arg *arg, int *fd);
void	exec_pipe_child2(t_node *node, t_arg *arg, int *fd);

int	exec_logical_operator(t_node *node, t_arg *arg)
{
	if (!strcmp(node->data, "&&"))
	{
		if (!start_exec(node->left, arg))
			if (!start_exec(node->right, arg))
				return (0);
			else
				return (1);
		else
			return (1);
	}
	else if (!strcmp(node->data, "||"))
	{
		if (start_exec(node->left, arg))
			if(!start_exec(node->right, arg))
				return (0);
			else
				return (1);
		else
			return (0);
	}
	return (1); // 위 두 경우가 아니라면 오류
}

int	exec_subshell(t_node *node, t_arg *arg)
{
	pid_t	pid;
	int		status;
	
	pid = 0;
	status = 0;
	if (!node->left)
		node = node->right;
	else
		node = node->left;
	pid = fork();
	if (!pid)
	{
		start_exec(node->left, arg);
		exec_perror("execve");
	}
	else
		waitpid(pid, &status, 0);
	return (status);
}

int	exec_pipeline(t_node *node, t_arg *arg)
{
	pid_t	pid[2];
	int		pipe_fd[2];

	arg->fork_sign++;
	if (pipe(pipe_fd) == -1)
		return (exec_perror("pipe"));
	pid[0] = fork();
	if (!pid[0])
		exec_pipe_child1(node, arg, pipe_fd);
	else
	{
		pid[1] = fork();
		if (!pid[1])
			exec_pipe_child2(node, arg, pipe_fd);
		else
			return (exec_parent(arg, pid, pipe_fd));
	}
	return (1); // 이 리턴을 쓰는경우는 자식프로세스가 제대로 종료되지않고 조건문을 나온경우이므로 오류처리.
}

int	exec_parent(t_arg *arg, int *pid, int *pipe)
{
	int		status;

	close(pipe[0]);
	close(pipe[1]);
	waitpid(pid[0], 0, 0);
	waitpid(pid[1], &status, 0);
	arg->fork_sign--;
	return (status);
}

void	exec_pipe_child1(t_node *node, t_arg *arg, int *pipe)
{
	close(pipe[0]);
	dup2(pipe[1], 1);
	close(pipe[1]);
	if (start_exec(node->left, arg))
		exit (1);
}

void	exec_pipe_child2(t_node *node, t_arg *arg, int *pipe)
{
	close(pipe[1]);
	dup2(pipe[0], 0);
	close(pipe[0]);
	if (start_exec(node->right, arg))
		exit (1);
}