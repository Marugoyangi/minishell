/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungwok <seungwok@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:44 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/21 17:58:36 by seungwok         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	built_in_env(t_node *node, t_env *env)
{
	t_env	*cur;

	if (node->argv[1])
	{
		write(2, "minishell: ", 11);
		write(2, node->argv[1], ft_strlen(node->argv[1]));
		write(2, ": No such file or directory\n", 28);
		return (127);
	}
	cur = env;
	while (cur)
	{
		if (cur->value)
			printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
	return (0);
}

int	built_in_export(t_node *node, t_env *env)
{
	if (!node->argv[1])
		return (export_none_arg(env));
	return (export_arg(node, env, 0, 0));
}
