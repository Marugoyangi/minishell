/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:51 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/08 16:52:18 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(char **path, char *command);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen2(const char *s);

void	exec_command(t_node *node, t_env *env, char **path)
{
	char	*excutable_path;
	char	**command_option;

	check_built_in(node, env);
	excutable_path = find_path(path, node->data);
	if (node->argv == 0)
	{
		node->argv = (char **)malloc(sizeof(char *) * 1);
		node->argv[0] = NULL;
	}
	command_option = node->argv;
	execve(excutable_path, command_option, 0);
	free(node->argv);
}

void	check_built_in(t_node *node, t_env *env)
{
	printf("hi?\n");
	if (!ft_strncmp(node->data, "echo", 4))
		built_in_echo(node->argv);
	else if (!ft_strncmp(node->data, "cd", 2))
		built_in_cd(node->argv);
	else if (!ft_strncmp(node->data, "pwd", 3))
		built_in_pwd();
	else if (!ft_strncmp(node->data, "export", 6))
		built_in_export(node, env);
	else if (!ft_strncmp(node->data, "unset", 5))
		built_in_unset(node, env);
	else if (!ft_strncmp(node->data, "env", 5))
		built_in_env(env);
	else if (!ft_strncmp(node->data, "exit", 4))
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
	return (command_path);
}

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char		*dest;
// 	size_t		str_len;
// 	size_t		i;

// 	str_len = ft_strlen2(s1) + ft_strlen2(s2);
// 	dest = (char *)malloc(sizeof(char) * (str_len + 1));
// 	if (dest == 0)
// 		return (0);
// 	str_len = 0;
// 	while (s1[str_len])
// 	{
// 		dest[str_len] = s1[str_len];
// 		++str_len;
// 	}
// 	i = 0;
// 	while (s2[i])
// 		dest[str_len++] = s2[i++];
// 	dest[str_len] = 0;
// 	return (dest);
// }
