/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_export.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 08:14:04 by woopinbell        #+#    #+#             */
/*   Updated: 2023/12/19 08:19:58 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	built_in_export(t_node *node, t_env *env)
{
	if (!node->argv[1])
		return (export_none_arg(env));
	return (export_arg(node, env, 0, 0));
}

int	export_none_arg(t_env *env)
{
	t_env	*dup;

	dup = dup_list(env);
	sort_list(dup);
	while (dup)
	{
		if (dup->value)
			printf("declare -x %s=\"%s\"\n", dup->key, dup->value);
		else
			printf("declare -x %s\n", dup->key);
		dup = dup->next;
	}
	free_list(dup);
	return (0);
}

int	export_arg(t_node *node, t_env *env, int i, int sign)
{
	char	**tmp;

	while (node->argv[++i])
	{
		if (ft_strchr(node->argv[i], '='))
		{
			tmp = ft_split(node->argv[i], '=');
			if (check_arg_name(tmp[0], &sign))
				printf("minishell: export: `%s': not a valid identifier\n", node->argv[i]);
			else
				export_arg_equal(env, tmp);
			free_split(tmp);
		}
		else
		{
			if (check_arg_name(node->argv[i], &sign))
				printf("minishell: export: `%s': not a valid identifier\n", node->argv[i]);
			else
				export_arg_not_equal(env, node->argv[i]);
		}
	}
	return (sign);
}

void	export_arg_equal(t_env *env, char **tmp)
{
	while (env)
	{
		if (!ft_strcmp(env->key, tmp[0]))
		{
			env->value = ft_strtrim(tmp[1], "\"");
			return ;
		}
		if (env->next == NULL)
			break ;
		env = env->next;
	}
	env->next = (t_env *)malloc(sizeof(t_env));
	env = env->next;
	env->key = ft_strdup(tmp[0]);
	env->value = ft_strtrim(tmp[1], "\"");
	env->next = NULL;
}

void	export_arg_not_equal(t_env *env, char *arg)
{
	while (env)
	{
		if (!ft_strcmp(env->key, arg))
		{
			env->value = NULL;
			return ;
		}
		if (env->next == NULL)
			break ;
		env = env->next;
	}
	env->next = (t_env *)malloc(sizeof(t_env));
	env = env->next;
	env->key = ft_strdup(arg);
	env->value = NULL;
	env->next = NULL;
}
