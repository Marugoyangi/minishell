/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:44 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/14 16:30:05 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	built_in_export(t_node *node , t_env *env);
int	export_none_arg(t_env *env);
int	built_in_unset(t_node *node , t_env *env);
int	built_in_env(t_env *env);

// export, unset 노드 생성시 args 인덱스값 설정의 의도성여부에 따른 수정필요
int	built_in_export(t_node *node , t_env *env)
{
	char	**tmp;
	
	if (!node->argv[1])
	{
		export_none_arg(env);
		return (0);
	}
	tmp = ft_split(node->argv[1], '=');
	while (env->next)
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

int	built_in_unset(t_node *node , t_env *env)
{	
	t_env	*tmp;

	if (!node->argv[1])
		free_list(env);
	else
	{
		while (env)
		{
			if (!strcmp(env->next->key, node->argv[1]))
			{
				tmp = env->next->next;
				free(env->next);
				env->next = tmp;
				break ;
			}
			env = env->next;
		}
	}
	return (0);
}

int	built_in_env(t_env *env)
{
	t_env	*cur;

	cur = env;
	while(cur)
	{
		printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
	return (0);
}
