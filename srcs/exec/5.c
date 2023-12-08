/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 18:54:16 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/08 16:16:03 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_subshell(t_node *node, t_env *env, char **path);
void	exec_pipeline(t_node *node, t_env *env, char **path);

void	exec_logical_operator(t_node *node, t_env *env, char **path)
{
	int		status;
	pid_t	pid;
	pid = fork();

	if (!pid)
	{
		start_exec(node->left, env, path);
		exit (0);
	}
	else
	{
		waitpid(pid, &status, 0);
		// 자식프로세스의 종료상대를 확인하기 위한 메크로
        // WIFEXITED, 자식 프로세스가 정상적으로 종료되었는지 확인 (정상 종료시 1)
        // WIFSTATUS, 자식 프로세스의 종료코드 (exit(?))를 추출, WIFEXITED가 참일때만 사용해야한다.
        // 시그널에 의한 종료인지, 정상적인 exit(0) 종료인지 확인을 위해 WIFSTATUS가 필요하다.
        if (node->data[0] == '&' && WIFEXITED(status) && !WEXITSTATUS(status))
            start_exec(node->right, env, path);
        if (node->data[0] == '|' && !WIFEXITED(status) && WEXITSTATUS(status))
            start_exec(node->right, env, path);
	}
}


// 서브쉘은 어떻게 노드에 입력되어있지?
// 상황에따라 start_exec or exec_commnad로 보낼예정.
void	exec_subshell(t_node *node, t_env *env, char **path)
{
	pid_t	pid;

	pid = fork();
	if (!pid)
	{
		start_exec(node->left, env, path);
		exit (0);
	}
	else
		waitpid(pid, 0, 0);
	exit (0);
}

void	exec_pipeline(t_node *node, t_env *env, char **path)
{
	pid_t	pid[2];
	int		fd[2];

	pipe(fd);
	pid[0] = fork();
	if (!pid[0])
	{
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		start_exec(node->left, env, path);
		exit(0);
	}
	else
	{
		pid[1] = fork();
		if (!pid[1])
		{
			close(fd[1]);
			dup2(fd[0], 0);
			close(fd[0]);
			start_exec(node->right, env, path);
			exit (0);
		}
		else
		{
			waitpid(pid[0], 0, 0);
			waitpid(pid[1], 0, 0);
		}
	}	
}
