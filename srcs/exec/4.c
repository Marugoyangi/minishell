/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:44 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/08 17:02:59 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_list(t_env *env);
void	free_list(t_env *env);
void	built_in_export(t_node *node , t_env *env);
void	built_in_unset(t_node *node , t_env *env);
void	built_in_env(t_env *env);

t_env *duplicate_list(t_env *env)
{
	t_env *head;
	t_env *cur;
	t_env *dup;

	head = (t_env *)malloc(sizeof(t_env));
	cur = head;
	dup = env;
	while(dup)
	{
		cur->key = env->key;
		cur->value = env->value;
		if (dup->next)
		{
			cur->next = (t_env *)malloc(sizeof(t_env));
			cur = cur->next;
		}
		dup = dup->next;
	}
	cur->next = 0;
	return (head);
}

t_env *dup_list(t_env *env)
{
	t_env *head;
	t_env *cur;
	t_env *dup;

	head = (t_env *)malloc(sizeof(t_env));
	dup = head;
	cur = env;
	while(cur)
	{
		dup->key = cur->key;
		dup->value = cur->value;
		if (cur->next)
		{
			dup->next = (t_env *)malloc(sizeof(t_env));
			cur = cur->next;
			dup = dup->next;
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
	t_env	*target;
	char	*tmp;
	
	cur = env;
	while (cur->next)
	{
		target = cur->next;
		while (target)
		{
			if (strcmp(cur->key, target->key) > 0)
			{
				tmp = cur->key;
				cur->key = target->key;
				target->key = tmp;
				tmp = cur->value;
				cur->value = target->value;
				target->value = tmp;
			}
			target = target->next;
		}
		cur = cur->next;
	}
}

void	free_list(t_env *env)
{
	t_env *cur;
	t_env *tmp;

	cur = env;
	while(cur)
	{
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
}

//	쌍따옴표를 써도되고 안써도되기때문에, 함수를 나누어서 사용하는게 좋아보인다.
void	built_in_export(t_node *node , t_env *env)
{
	t_env	*dup;
	t_env	*cur;
	char	**tmp;
	
	// if (!node->argv[0])
	// {
			printf("hi \n");
	print_env(env);
		dup = dup_list(env);
		// sort_list(dup);
		cur = dup;
		while (cur)
		{
			printf("declare -x %s=\"%s\"\n", dup->key, dup->value);
			cur = cur->next;
		}
		free_list(dup);
		return ;
	// }
	cur = env;
	while (cur)
		cur = cur->next;
	cur = (t_env *)malloc(sizeof(t_env));
	tmp = ft_split(node->argv[0], '=');
	cur->key = tmp[0];
	cur->value = tmp[1];
	cur->next = 0;
}

void	built_in_unset(t_node *node , t_env *env)
{	
	t_env	*cur;
	t_env	*tmp;

	if (!node->argv[0])
		free_list(env);
	else
	{
		cur = env;
		while (cur)
		{
			if (!strcmp(cur->next->key, node->argv[0]))
			{
				tmp = cur->next->next;
				free(cur->next);
				cur->next = tmp;
			}
			cur = cur->next;
		}
	}
}

void	built_in_env(t_env *env)
{
	t_env	*cur;

	cur = env;
	while(cur)
	{
		printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
}
