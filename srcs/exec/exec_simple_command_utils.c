/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:52:02 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/20 00:49:31 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_perror(char *str, int sign)
{
	if (sign == 1)
	{
		write(2, "minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": command not found\n", 20);
		exit(127);
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
