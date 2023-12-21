/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:52:02 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/21 22:21:02 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_perror(char *str, int sign)
{
	struct stat	file_info;

	file_info = (struct stat){0};
	write(2, "minishell: ", 11);
	if (sign == 1)
	{
		stat(str, &file_info);
		write(2, str, ft_strlen(str));
		if (S_ISDIR(file_info.st_mode))
		{
			write(2, ": Is a directory\n", 17);
			exit (126);
		}
		else if (str)
		{
			write(2, ": No such file or directory\n", 28);
			while (*(str + 1))
				str++;
			if (*str == '/')
				exit (126);
			exit (127);
		}
	}
	perror(str);
	return (1);
}

char	**set_path(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur && ft_strcmp("PATH", cur->key))
		cur = cur->next;
	if (!cur)
		return (0);
	return (ft_split(cur->value, ':'));
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
			return (try_executable);
		}
		free(command_path);
		free(try_executable);
		path++;
	}
	return (0);
}

char	**make_envp(t_env *envp_head)
{
	int		i;
	char	**envp;
	t_env	*cur;

	i = 0;
	cur = envp_head;
	while (cur)
	{
		cur = cur->next;
		i++;
	}
	envp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!envp)
		return (NULL);
	i = 0;
	cur = envp_head;
	while (cur)
	{
		envp[i] = ft_strjoin(cur->key, "=");
		envp[i] = modified_strjoin(envp[i], cur->value, 1);
		cur = cur->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
