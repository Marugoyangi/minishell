/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:51 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/21 22:27:10 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_command(t_node *node, t_arg *arg)
{
	int		status;
	char	**path;

	status = 0;
	status = check_built_in(node, arg);
	if (status == -1)
	{
		path = set_path(arg->envp_head);
		if (!path && node->data && (!(!ft_strncmp(node->data, "./", 2)
					|| !ft_strncmp(node->data, "../", 3)
					|| !ft_strncmp(node->data, "/", 1))))
		{
			write(2, "minishell: ", 11);
			write(2, node->data, ft_strlen(node->data));
			write(2, ": command not found\n", 20);
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
		pid = fork();
		if (pid == -1)
			return (fail_to_fork(arg, 0));
		if (!pid)
			external_command_child(node, arg, path);
		else if (node->data && ft_strstr(node->data, "/minishell\0"))
			signal_ignore();
		else if (node->data && !ft_strstr(node->data, "/minishell\0"))
		{
			signal(SIGINT, sig_handler_exec);
			signal(SIGQUIT, sig_handler_exec);
		}
		waitpid(pid, &status, 0);
		waitpid_signal(arg, &status);
		return (status);
	}
	else
		exec_check_path(node, arg, path);
	return (1);
}

void	external_command_child(t_node *node, t_arg *arg, char **path)
{
	arg->term.c_lflag |= ECHOCTL;
	tcsetattr(STDOUT_FILENO, TCSANOW, &arg->term);
	signal(SIGINT, sig_handler_waiting);
	signal(SIGQUIT, sig_handler_waiting);
	exec_check_path(node, arg, path);
}

void	exec_check_path(t_node *node, t_arg *arg, char **path)
{
	char	*excutable_path;
	char	**envp;

	envp = make_envp(arg->envp_head);
	if (node->data && (!ft_strncmp(node->data, "./", 2) || \
	!ft_strncmp(node->data, "../", 3) || !ft_strncmp(node->data, "/", 1)))
	{
		execve(node->argv[0], node->argv, envp);
		exec_perror(node->data, 1);
	}
	else
	{
		excutable_path = find_path(path, node->data);
		if (!excutable_path)
			exec_perror(node->data, 1);
		execve(excutable_path, node->argv, envp);
		free(excutable_path);
		if (node->data)
			exec_perror(node->data, 1);
		else
			exit (0);
	}
	free_split(envp);
}
