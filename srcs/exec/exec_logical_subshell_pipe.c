/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_logical_subshell_pipe.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 18:54:16 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/10 01:28:54 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		exec_logical_operator(t_node *node, t_env *env, char **path);
int		exec_subshell(t_node *node, t_env *env, char **path);
int		exec_pipeline(t_node *node, t_env *env, char **path);
void	exec_pipe_child1(t_node *node, t_env *env, char **path, int *fd);
void	exec_pipe_child2(t_node *node, t_env *env, char **path, int *fd);

// 논리연산자는 포크없이 구현해야한다.
int	exec_logical_operator(t_node *node, t_env *env, char **path)
{
	if (!strcmp(node->data, "&&") == 0)
	{
		if (!start_exec(node->left, env, path))
			if (!start_exec(node->right, env, path))
				return (0);
			else
				return (1);
		else
			return (1);
	}
	else if (!strcmp(node->data, "||"))
	{
		if (start_exec(node->left, env, path))
			if(!start_exec(node->right, env, path))
				return (0);
			else
				return (1);
		else
			return (0);
	}
	return (1); // 위 두 경우가 아니라면 오류
}

int	exec_subshell(t_node *node, t_env *env, char **path)
{
	pid_t	pid;
	int		status;
	pid = fork();
	if (!pid)
	{
		start_exec(node->left, env, path);
		exit (0);
	}
	else
		waitpid(pid, &status, 0);
	return (status);
}

int	exec_pipeline(t_node *node, t_env *env, char **path)
{
	pid_t	pid[2];
	int		fd[2];
	int		status;

	pipe(fd);
	pid[0] = fork();
	if (!pid[0])
		exec_pipe_child1(node, env, path, fd);
	else
	{
		pid[1] = fork();
		if (!pid[1])
			exec_pipe_child2(node, env, path, fd);
		else
		{
			waitpid(pid[0], 0, 0);
			waitpid(pid[1], &status, 0);
			return (status);
		}
	}
	return (1); // 이 리턴을 쓰는경우는 자식프로세스가 제대로 종료되지않고 조건문을 나온경우이므로 오류처리.
}

void	exec_pipe_child1(t_node *node, t_env *env, char **path, int *fd)
{
	close(fd[0]);
	dup2(fd[1], 1);
	close(fd[1]);
	if (start_exec(node->left, env, path))
		exit (1);
	exit(0);
}

void	exec_pipe_child2(t_node *node, t_env *env, char **path, int *fd)
{
	close(fd[1]);
	dup2(fd[0], 0);
	close(fd[0]);
	if (start_exec(node->right, env, path))
		exit (1);
	exit (0);
}