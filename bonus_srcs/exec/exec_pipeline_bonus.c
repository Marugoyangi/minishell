/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:51:18 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:26:38 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	exec_pipeline(t_node *node, t_arg *arg)
{
	pid_t	pid[2];
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (exec_perror("pipe", 0));
	arg->fork_sign++;
	pid[0] = fork();
	if (pid[0] == -1)
		return (fail_to_fork(arg, 0));
	if (!pid[0])
		exec_pipe_child1(node, arg, pipe_fd);
	else
	{
		pid[1] = fork();
		if (pid[1] == -1)
			return (fail_to_fork(arg, pid[0]));
		else if (pid[1] == 0)
			exec_pipe_child2(node, arg, pipe_fd);
		else
			return (exec_parent(arg, pid, pipe_fd));
	}
	return (1);
}

int	fail_to_fork(t_arg *arg, int pid)
{
	if (pid > 0)
		kill(pid, SIGKILL);
	if (arg->fork_sign)
		arg->fork_sign--;
	return (exec_perror("fork", 0));
}

int	exec_parent(t_arg *arg, int *pid, int *pipe)
{
	int		status;

	close(pipe[0]);
	close(pipe[1]);
	signal(SIGINT, sig_handler_exec);
	signal(SIGQUIT, sig_handler_exec);
	waitpid(pid[0], 0, 0);
	waitpid(pid[1], &status, 0);
	terminal_interactive(arg);
	arg->fork_sign--;
	return (WEXITSTATUS(status));
}

void	exec_pipe_child1(t_node *node, t_arg *arg, int *pipe)
{
	close(pipe[0]);
	dup2(pipe[1], 1);
	close(pipe[1]);
	arg->term.c_lflag |= ECHOCTL;
	tcsetattr(STDOUT_FILENO, TCSANOW, &arg->term);
	signal(SIGINT, sig_handler_waiting);
	signal(SIGQUIT, sig_handler_waiting);
	if (start_exec(node->left, arg))
		exit (1);
	else
		exit (0);
}

void	exec_pipe_child2(t_node *node, t_arg *arg, int *pipe)
{
	close(pipe[1]);
	dup2(pipe[0], 0);
	close(pipe[0]);
	arg->term.c_lflag |= ECHOCTL;
	arg->last_exit_status = 0;
	tcsetattr(STDOUT_FILENO, TCSANOW, &arg->term);
	signal(SIGINT, sig_handler_waiting);
	signal(SIGQUIT, sig_handler_waiting);
	if (start_exec(node->right, arg))
		exit (1);
	else
		exit (0);
}
