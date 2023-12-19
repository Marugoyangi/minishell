/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_cd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:50:29 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/20 03:24:36 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	built_in_cd(t_node *node, t_arg *arg, char **argv)
{
	if (!argv[1])
		return (0);
	if (argv[2])
	{
		write(2, "minishell: cd: too many arguments\n", 35);
		return (1);
	}
	if (!ft_strcmp(argv[1], "-"))
		return (built_in_cd_oldpwd(node, arg));
	if (chdir(argv[1]) == -1)
		exec_perror("cd", 0);
	if (arg->oldpwd)
		free(arg->oldpwd);
	arg->oldpwd = ft_strdup(arg->pwd);
	built_in_cd_set_pwd(node, arg);
	return (0);
}

void	cd_get_pwd(char *tmp, t_env *cur, t_arg *arg, t_node *node)
{
	if (!tmp)
	{
		if (!cur)
			perror("minishell");
		arg->pwd = modified_strjoin(arg->pwd, "/", 1);
		arg->pwd = modified_strjoin(arg->pwd, node->argv[1], 1);
	}
	else
	{
		free(arg->pwd);
		arg->pwd = tmp;
	}
}

void	built_in_cd_set_pwd(t_node *node, t_arg *arg)
{
	char	*tmp;
	t_env	*cur;

	cur = find_env_node(arg->envp_head, "PWD");
	if (cur)
	{
		free(cur->value);
		cur->value = getcwd(0, 0);
		if (!cur->value)
		{
			exec_perror("cd", 0);
			cur->value = ft_strdup("");
		}
	}
	tmp = getcwd(0, 0);
	cd_get_pwd(tmp, cur, arg, node);
}

int	built_in_cd_oldpwd(t_node *node, t_arg *arg)
{
	t_env	*cur;
	char	*tmp;

	cur = find_env_node(arg->envp_head, "OLDPWD");
	if (!cur || !ft_strcmp(cur->value, ""))
	{
		printf("minishell: cd: OLDPWD not set\n");
		return (1);
	}
	tmp = getcwd(0, 0);
	if (!tmp)
		return (exec_perror("cd", 0));
	if (chdir(arg->oldpwd) == -1)
	{
		free(tmp);
		return (exec_perror("cd", 0));
	}
	free(cur->value);
	free(arg->oldpwd);
	cur->value = tmp;
	arg->oldpwd = ft_strdup(tmp);
	built_in_cd_set_pwd(node, arg);
	return (0);
}

int	error_getcwd(t_env *env, char **argv)
{
	t_env	*cur;

	cur = env;
	while (cur && ft_strcmp(cur->key, "PWD"))
		cur = cur->next;
	cur->value = modified_strjoin(cur->value, "/", 1);
	cur->value = modified_strjoin(cur->value, argv[1], 1);
	chdir(argv[1]);
	return (exec_perror("getcwd", 0));
}
