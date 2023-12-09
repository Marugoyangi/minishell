/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:35:44 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/10 01:28:44 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	built_in_export(t_node *node , t_env *env);
int	export_none_arg(t_env *env);
int	built_in_unset(t_node *node , t_env *env);
int	built_in_env(t_env *env);

int	built_in_export(t_node *node , t_env *env)
{
	t_env	*cur;
	char	**tmp;
	
	if (!node->argv[1])
	{
		export_none_arg(env);
		return (0);
	}
	while (cur)
		cur = cur->next;
	cur = (t_env *)malloc(sizeof(t_env));
	tmp = ft_split(node->argv[0], '=');
	cur->key = tmp[0];
	cur->value = ft_strtrim(tmp[1], "\"");
	cur->next = 0;
	free(tmp);
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
	t_env	*cur;
	t_env	*tmp;

	if (!node->argv[0])
		free_list(env);
	else
	{
		cur = env;
		while (cur)
		{
			if (!strcmp(cur->next->key, node->argv[1]))
			{
				tmp = cur->next->next;
				free(cur->next);
				cur->next = tmp;
			}
			cur = cur->next;
		}
	}
	return (0);
}

// 환경변수의 임시적용을 위해, env 리스트를 복사하여 사용.
// 구현한 예시가 맞다면 norm을 위해 함수 두개로 분기 예정.
// 최종 수정, 옵션/인자 둘 다 없는 env
// 
int	built_in_env(t_env *env)
{
	t_env	*cur;

	cur = env;
	while(cur)
	{
		printf("%s=%s\n", cur->key, ft_strtrim(cur->value, "\""));
		cur = cur->next;
	}
	return (0);
}
