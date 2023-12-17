/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   exec_pipeline.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: woopinbell <woopinbell@student.42.fr>	  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/12/04 18:54:16 by seungwok		  #+#	#+#			 */
/*   Updated: 2023/12/15 12:38:30 by woopinbell	   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

int	exec_pipeline(t_node *node, t_arg *arg)
{
	pid_t	pid[2];
	int		pipe_fd[2];

	arg->fork_sign++;
	if (pipe(pipe_fd) == -1)
		return (exec_perror("minishell"));
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
	return (1);
}

int	exec_parent(t_arg *arg, int *pid, int *pipe)
{
	int		status;

	close(pipe[0]);
	close(pipe[1]);
	waitpid(pid[0], 0, 0);
	waitpid(pid[1], &status, 0);
	arg->fork_sign--;
	return (WEXITSTATUS(status));
}

void	exec_pipe_child1(t_node *node, t_arg *arg, int *pipe)
{
	close(pipe[0]);
	dup2(pipe[1], 1);
	close(pipe[1]);
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
	if (start_exec(node->right, arg))
		exit (1);
	else
		exit (0);
}
