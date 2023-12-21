/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_cd_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungwok <seungwok@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:50:29 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/21 08:49:42 by seungwok         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
	built_in_cd_set_pwd_env(node, arg);
	return (0);
}

void	built_in_cd_set_pwd_arg(char *tmp, t_env *cur, t_arg *arg, t_node *node)
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

void	built_in_cd_set_pwd_env(t_node *node, t_arg *arg)
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
	built_in_cd_set_pwd_arg(tmp, cur, arg, node);
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
	built_in_cd_set_pwd_env(node, arg);
	return (0);
}
