/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 03:51:05 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/05 03:51:05 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_env(t_env *env_head, char *key)
{
	t_env	*tmp;

	tmp = env_head;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	print_env(t_env *env_head)
{
	t_env	*tmp;

	tmp = env_head;
	while (tmp)
	{
		printf("key: %s\n", tmp->key);
		printf("value: %s\n", tmp->value);
		tmp = tmp->next;
	}
}

t_env	*create_env(char *key, char *value)
{
	t_env	*env;

	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->key = ft_strdup(key);
	env->value = ft_strdup(value);
	env->next = NULL;
	return (env);
}

t_env	*init_envp(char **envp)
{
	int		i;
	t_env	*env_head;
	char	**tmp_split;
	t_env	*tmp_node;

	i = 0;
	env_head = create_env("PS1", "\033[1;34mminishell \033[0;33m✗ \033[0m");
	tmp_node = env_head;
	while (tmp_node && envp && envp[i])
	{
		tmp_split = ft_split(envp[i], '=');
		tmp_node->next = create_env(tmp_split[0], tmp_split[1]);
		free_split((void **)tmp_split);
		tmp_node = tmp_node->next;
		i++;
	}
	return (env_head);
}