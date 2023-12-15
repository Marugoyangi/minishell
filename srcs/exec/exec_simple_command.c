/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:51 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/15 14:21:55 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		exec_command(t_node *node, t_arg *arg);
int		external_command(t_node *node, t_arg *arg, char **path);
void	external_command_child(t_node *node, t_arg *arg, char **path);
void	exec_check_path(t_node *node, t_arg *arg, char **path);

int	exec_command(t_node *node, t_arg *arg)
{
	int		status;
	char	**path;

	status = check_built_in(node, arg);
	if (status == -1)
	{
		path = set_path(arg->envp_head);
		if (!path)
		{
			printf("minishell: %s: 그런 파일이나 디렉터리가 없습니다.\n", node->data);
			return (127);
		}
		status = external_command(node, arg, path);
	}
	else if (!(status == -1) && arg->fork_sign == 1)
		exit(status);
	return (status);
}

int	external_command(t_node *node, t_arg *arg, char **path)
{
	int		status;
	pid_t	pid;

	status = 0;
	if (!arg->fork_sign)
	{
		if (ft_strstr(node->data, "./minishell"))
			g_signal_fork = 1;
		pid = fork();
		if (!pid)
			external_command_child(node, arg, path);
		else
		{
			terminal_default(0, arg);
			waitpid(pid, &status, 0);
			g_signal_fork = 0;
			terminal_interactive(arg);
			return (WEXITSTATUS(status));
		}
	}
	else
		exec_check_path(node, arg, path);
	return (1);
}

void	external_command_child(t_node *node, t_arg *arg, char **path)
{
	terminal_default(0, arg);
	exec_check_path(node, arg, path);
	exit (1);
}

void	exec_check_path(t_node *node, t_arg *arg, char **path)
{
	char	*excutable_path;

	if (ft_strncmp(node->data, "./", 2)
		|| ft_strncmp(node->data, "../", 3
			|| ft_strncmp(node->data, "/", 1)))
	{
		excutable_path = find_path(path, node->data);
		if (!excutable_path)
			exec_perror("execve");
		execve(excutable_path, node->argv, make_envp(arg->envp_head));
		exec_perror("execve");
	}
	else
	{
		execve(node->argv[0], node->argv, make_envp(arg->envp_head));
		excutable_path = find_path(path, node->data);
		if (!excutable_path)
			exec_perror("execve");
		execve(excutable_path, node->argv, make_envp(arg->envp_head));
		exec_perror("execve");
	}
}
