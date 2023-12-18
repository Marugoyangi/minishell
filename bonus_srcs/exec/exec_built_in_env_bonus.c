/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_env_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:44 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/19 07:57:19 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	built_in_export(t_node *node, t_env *env)
{
	char	**tmp;

	if (!node->argv[1])
	{
		export_none_arg(env);
		return (0);
	}
	tmp = ft_split(node->argv[1], '=');
	while (env->next || !ft_strcmp(env->key, tmp[0]))
	{
		if (!ft_strcmp(env->key, tmp[0]))
		{
			env->value = ft_strtrim(tmp[1], "\"");
			free_split(tmp);
			return (0);
		}
		env = env->next;
	}
	env->next = (t_env *)malloc(sizeof(t_env));
	env = env->next;
	env->key = ft_strdup(tmp[0]);
	env->value = ft_strtrim(tmp[1], "\"");
	env->next = NULL;
	free_split(tmp);
	return (0);
}

int	export_none_arg(t_env *env)
{
	t_env	*dup;

	dup = dup_list(env);
	sort_list(dup);
	while (dup)
	{
		printf("declare -x %s=\"%s\"\n", dup->key, dup->value);
		dup = dup->next;
	}
	free_list(dup);
	return (0);
}

int	built_in_unset(t_node *node, t_arg *arg)
{
	t_env	*cur;
	int		i;

	cur = arg->envp_head;
	if (node->argv[1])
	{
		i = 1;
		while (node->argv[i])
		{
			built_in_unset_iter(node, cur, arg, i);
			i++;
		}
	}
	return (0);
}

void	built_in_unset_iter(t_node *node, t_env	*cur, t_arg *arg, int i)
{
	t_env	*tmp;

	if (!ft_strcmp(cur->key, node->argv[i]))
	{
		tmp = cur->next;
		free_env_node(cur);
		arg->envp_head = tmp;
	}
	else
	{
		while (cur->next)
		{
			if (!ft_strcmp(cur->next->key, node->argv[i]))
			{
				tmp = cur->next->next;
				free_env_node(cur->next);
				cur->next = tmp;
				break ;
			}
			cur = cur->next;
		}
	}
}

int	built_in_env(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
	return (0);
}
