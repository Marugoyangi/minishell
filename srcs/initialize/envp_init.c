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

	i = -1;
	if (tgetnum("colors"))
		head = create_env("PS1", "\033[32m➜ \033[36mminishell \033[33m✗ \033[0m");
	else
		head = create_env("PS1", "➜  minishell ✗ ");
	head->next = create_env("PS2", "> ");
	tmp_node = head->next;
	while (tmp_node && envp && envp[++i])
	{
		split = ft_split(envp[i], '=');
		if (ft_strcmp(split[0], "PS1") && ft_strcmp(split[0], "PS2")
			&& ft_strcmp(split[0], "OLDPWD"))
		{
			tmp_node->next = create_env(split[0], split[1]);
			tmp_node = tmp_node->next;
		}
		free_split(split);
	}
	return (head);
}
