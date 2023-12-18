/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_cd_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:50:29 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/19 03:30:09 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	built_in_cd_set_env(t_env *env)
{
	if (env)
	{
		free(env->value);
		env->value = getcwd(0, 0);
	}
}

int	built_in_cd_oldpwd(t_env *env, char **argv)
{
	t_env	*cur;
	char	*tmp;

	cur = env;
	while (cur && ft_strcmp(cur->key, "OLDPWD"))
		cur = cur->next;
	if (!cur || !ft_strcmp(cur->value, "") || argv[2])
	{
		printf("minishell: cd: OLDPWD not set or too many arguments\n");
		return (1);
	}
	tmp = getcwd(0, 0);
	if (chdir(cur->value) == -1)
	{
		free(tmp);
		perror("minishell");
		return (1);
	}
	free(cur->value);
	cur->value = tmp;
	cur = env;
	while (cur && ft_strcmp(cur->key, "PWD"))
		cur = cur->next;
	built_in_cd_set_env(cur);
	return (0);
}

int	built_in_cd(t_env *env, char **argv)
{
	t_env	*cur;

	if (!ft_strcmp(argv[1], "-"))
		return (built_in_cd_oldpwd(env, argv));
	cur = env;
	while (cur && ft_strcmp(cur->key, "OLDPWD"))
		cur = cur->next;
	built_in_cd_set_env(cur);
	if (chdir(argv[1]) == -1)
	{
		if (cur)
		{
			free(cur->value);
			cur->value = ft_strdup("");
		}
		perror("minishell");
		return (1);
	}
	cur = env;
	while (cur && ft_strcmp(cur->key, "PWD"))
		cur = cur->next;
	built_in_cd_set_env(cur);
	return (0);
}
