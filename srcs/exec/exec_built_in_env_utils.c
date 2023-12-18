/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_env_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:50:38 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/18 15:50:38 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	check_arg_name(char *str, int *sign)
{
	int	i;

	i = 0;
	if (!(str[i] >= 'a' && str[i] <= 'z') && !(str[i] >= 'A' && str[i] <= 'Z'))
	{
		*sign = 1;
		return (1);
	}
	while (str[++i])
	{
		if (i == 256)
		{
			*sign = 1;
			return (1);
		}
		if (!(str[i] >= 'a' && str[i] <= 'z')
			&& !(str[i] >= 'A' && str[i] <= 'Z')
			&& !(str[i] >= '0' && str[i] <= '9') && str[i] != '_')
		{
			*sign = 1;
			return (1);
		}
	}
	return (0);
}
