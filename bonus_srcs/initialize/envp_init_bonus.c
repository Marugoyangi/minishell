/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_init_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:24:44 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:24:44 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	set_env(t_env *env_head, char *key, char *value)
{
	t_env	*tmp;

	tmp = env_head;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
}

char	*find_env(t_env *env_head, char *key)
{
	t_env	*tmp;

	tmp = env_head;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

t_env	*create_env(char *key, char *value)
{
	t_env	*env;

	env = (t_env *)ft_malloc(sizeof(t_env));
	env->key = ft_strdup(key);
	env->value = ft_strdup(value);
	env->next = NULL;
	return (env);
}

void	append_env(t_env *env_head, char *key, char *value)
{
	t_env	*tmp;

	tmp = env_head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = create_env(key, value);
}

t_env	*init_envp(char **envp)
{
	int		i;
	t_env	*head;
	char	**split;
	t_env	*tmp_node;

	i = 0;
	if (!envp)
		return (NULL);
	split = ft_split(envp[i], '=');
	head = create_env(split[0], split[1]);
	free_split(split);
	tmp_node = head;
	while (tmp_node && envp[++i])
	{
		split = ft_split(envp[i], '=');
		if (ft_strcmp(split[0], "OLDPWD"))
		{
			tmp_node->next = create_env(split[0], split[1]);
			tmp_node = tmp_node->next;
		}
		free_split(split);
	}
	return (head);
}
