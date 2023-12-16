/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungwok <seungwok@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:51 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/16 17:32:34 by seungwok         ###   ########seoul.kr  */
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
			write(2, "minishell: ", 11);
			write(2, node->data, ft_strlen(node->data));
			write(2, ": command not foudn\n", 20);
			return (127);
		}
		status = external_command(node, arg, path);
		free_split(path);
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
		if (node->data && ft_strstr(node->data, "/minishell\0"))
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
	char	**envp;

	envp = make_envp(arg->envp_head);
	if (ft_strncmp(node->data, "./", 2) || ft_strncmp(node->data, "../", 3)
		|| ft_strncmp(node->data, "/", 1))
	{
		excutable_path = find_path(path, node->data);
		if (!excutable_path)
			exec_perror("execve");
		execve(excutable_path, node->argv, envp);
		exec_perror("execve");
	}
	else
	{
		execve(node->argv[0], node->argv, envp);
		excutable_path = find_path(path, node->data);
		if (!excutable_path)
			exec_perror("execve");
		execve(excutable_path, node->argv, envp);
		exec_perror("execve");
	}
	if (excutable_path)
		free(excutable_path);
	free_split(envp);
}
