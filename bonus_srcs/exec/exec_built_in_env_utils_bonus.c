/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_env_utils_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 03:30:14 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/19 03:30:14 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_env	*dup_list(t_env *env)
{
	t_env	*head;
	t_env	*dup;

	head = (t_env *)malloc(sizeof(t_env));
	dup = head;
	while (env)
	{
		dup->key = env->key;
		dup->value = env->value;
		if (env->next)
		{
			dup->next = (t_env *)malloc(sizeof(t_env));
			dup = dup->next;
			env = env->next;
		}
		else
		{
			dup->next = 0;
			break ;
		}
	}
	return (head);
}

void	sort_list(t_env *env)
{
	t_env	*cur;
	t_env	*comparison;
	char	*tmp;

	cur = env;
	while (cur->next)
	{
		comparison = cur->next;
		while (comparison)
		{
			if (ft_strcmp(cur->key, comparison->key) > 0)
			{
				tmp = cur->key;
				cur->key = comparison->key;
				comparison->key = tmp;
				tmp = cur->value;
				cur->value = comparison->value;
				comparison->value = tmp;
			}
			comparison = comparison->next;
		}
		cur = cur->next;
	}
}

void	free_list(t_env *env)
{
	t_env	*cur;
	t_env	*tmp;

	cur = env;
	while (cur)
	{
		tmp = cur->next;
		free_env_node(cur);
		cur = tmp;
	}
}

void	free_env_node(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env);
}
