/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:44 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/16 05:24:48 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		built_in_export(t_node *node, t_env *env);
int		export_none_arg(t_env *env);
int		built_in_unset(t_node *node, t_arg *arg);
void	built_in_unset_iter(t_node *node, t_env	*cur, t_arg *arg, int i);
int		built_in_env(t_env *env);

int	built_in_export(t_node *node, t_env *env)
{
	char	**tmp;

	if (!node->argv[1])
	{
		export_none_arg(env);
		return (0);
	}
	tmp = ft_split(node->argv[1], '=');
	while (env->next || !strcmp(env->key, tmp[0]))
	{
		if (!strcmp(env->key, tmp[0]))
		{
			env->value = ft_strtrim(tmp[1], "\"");
			free(tmp);
			return (0);
		}
		env = env->next;
	}
	env->next = (t_env *)malloc(sizeof(t_env));
	env = env->next;
	env->key = tmp[0];
	env->value = ft_strtrim(tmp[1], "\"");
	env->next = NULL;
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
	if (!node->argv[1])
	{
		free_list(cur);
		arg->envp_head = 0;
	}
	else
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

	if (!strcmp(cur->key, node->argv[i]))
	{
		tmp = cur->next;
		free_env_node(cur);
		arg->envp_head = tmp;
	}
	else
	{
		while (cur->next)
		{
			if (!strcmp(cur->next->key, node->argv[i]))
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
