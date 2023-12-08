/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungwok <seungwok@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:51 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/06 20:02:43 by seungwok         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_path(char **path, char *command);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen2(const char *s);

void	exec_commnad(t_node *node, t_env *env, char **path)
{
	char	*excutable_path;
	char	*command_option;
	int		i;

	check_built_in(node, env);
	excutable_path = find_path(path, node->data);
	command_option = node->data;
	i = 0;
	while (node->argv[i])
	{
		command_option = ft_strjoin(command_option, node->argv[i]);
		i++;
	}
	execve(excutable_path, command_option, 0);
}

void	check_built_in(t_node *node, t_env *env)
{
	if (ft_strncmp(node->data, "echo", 4))
		built_in_echo(node->argv);
	else if (ft_strncmp(node->data, "cd", 2))
		built_in_cd(node->argv);
	else if (ft_strncmp(node->data, "pwd", 3))
		built_in_pwd();
	else if (ft_strncmp(node->data, "export", 6))
		built_in_export(node, env);
	else if (ft_strncmp(node->data, "unset", 5))
		built_in_unset(node, env);
	else if (ft_strncmp(node->data, "env", 5))
		built_in_env(env);
	else if (ft_strncmp(node->data, "exit", 4))
		built_in_exit();
	return ;
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
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char		*dest;
	size_t		str_len;
	size_t		i;

	str_len = ft_strlen2(s1) + ft_strlen2(s2);
	dest = (char *)malloc(sizeof(char) * (str_len + 1));
	if (dest == 0)
		return (0);
	str_len = 0;
	while (s1[str_len])
	{
		dest[str_len] = s1[str_len];
		++str_len;
	}
	i = 0;
	while (s2[i])
		dest[str_len++] = s2[i++];
	dest[str_len] = 0;
	return (dest);
}
