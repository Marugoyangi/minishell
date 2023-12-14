/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:51 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/15 00:29:25 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		exec_command(t_node *node, t_arg *arg);
int		check_built_in(t_node *node, t_arg *arg);
char	**set_path(t_env *env);
void	exec_check_path(t_node *node, t_arg *arg, char **path);
int		external_command(t_node *node, t_arg *arg, char **path);
char	*find_path(char **path, char *command);


// 정상종료시 0, 그 외 나머지 값 출력.
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

// PATH 환경변수 찾아서, 경로배열 스플릿 후 반환.
char	**set_path(t_env *env)
{
	t_env *cur;

	cur = env;
	while (cur && strcmp("PATH", cur->key))
		cur = cur->next;
	if (!cur)
		return (0);
	return (ft_split(cur->value, ':'));
}

void	exec_check_path(t_node *node, t_arg *arg, char **path)
{
	char	*excutable_path;

	if (ft_strncmp(node->data, "./", 2) || ft_strncmp(node->data, "../", 3 || ft_strncmp(node->data, "/", 1)))
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
		{
			terminal_default(0, arg);
			exec_check_path(node, arg, path);
			exit (0);
		}
		else
		{
			terminal_default(0, arg);
			waitpid(pid, &status, 0);
			g_signal_fork = 0;
			terminal_interactive(arg);
		}
		return (WEXITSTATUS(status));
	}
	else
		exec_check_path(node, arg, path);
	return (1);
}

int	check_built_in(t_node *node, t_arg *arg)
{
	if (!ft_strcmp(node->data, "echo"))
		return (built_in_echo(node->argv));
	else if (!ft_strcmp(node->data, "cd"))
		return (built_in_cd(node->argv));
	else if (!ft_strcmp(node->data, "pwd"))
		return (built_in_pwd());
	else if (!ft_strcmp(node->data, "exit"))
		built_in_exit(node);
	else if (!ft_strcmp(node->data, "export"))
		return (built_in_export(node, arg->envp_head));
	else if (!ft_strcmp(node->data, "unset"))
		return (built_in_unset(node, arg));
	else if (!ft_strcmp(node->data, "env"))
		return (built_in_env(arg->envp_head));
	return (-1);
}

char	*find_path(char **path, char *command)
{
	char	*command_path;
	char	*try_executable;
	
	while (*path)
	{
		command_path = ft_strjoin(*path, "/");
		try_executable = ft_strjoin(command_path, command);
		if (!access(try_executable, X_OK))
		{
			free(command_path);
			return(try_executable);
		}
		free(command_path);
		free(try_executable);
		path++;
	}
	return (command_path);
}
