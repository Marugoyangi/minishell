/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   exec_simple_command_utils.c						:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: woopinbell <woopinbell@student.42.fr>	  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/12/15 13:17:09 by woopinbell		#+#	#+#			 */
/*   Updated: 2023/12/15 13:21:36 by woopinbell	   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

int		exec_perror(char *str);
char	**set_path(t_env *env);
char	*find_path(char **path, char *command);
char	**make_envp(t_env *envp_head);

int	exec_perror(char *str)
{
	perror("minishell");
	if (!ft_strcmp(str, "execve"))
		exit(1);
	return (1);
}

char	**set_path(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur && strcmp("PATH", cur->key))
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
	return (command);
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
