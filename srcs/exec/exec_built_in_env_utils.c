/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_env_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 02:43:33 by marvin            #+#    #+#             */
/*   Updated: 2023/12/09 02:43:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			if (strcmp(cur->key, comparison->key) > 0)
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
