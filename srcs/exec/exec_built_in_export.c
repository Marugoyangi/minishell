/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_export.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungwok <seungwok@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 08:14:04 by woopinbell        #+#    #+#             */
/*   Updated: 2023/12/21 17:59:42 by seungwok         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_arg_name(char *str, int *sign)
{
	int	i;

	i = 0;
	if (!(str[i] >= 'a' && str[i] <= 'z') && !(str[i] >= 'A' && str[i] <= 'Z'))
	{
		if (sign)
			*sign = 1;
		return (1);
	}
	while (str[++i])
	{
		if (i == 256 || (!(str[i] >= 'a' && str[i] <= 'z')
				&& !(str[i] >= 'A' && str[i] <= 'Z')
				&& !(str[i] >= '0' && str[i] <= '9') && str[i] != '_'))
		{
			if (sign)
				*sign = 1;
			return (1);
		}
	}
	return (0);
}

int	export_none_arg(t_env *env)
{
	t_env	*dup;
	t_env	*tmp;

	if (!env)
		return (0);
	dup = dup_list(env);
	tmp = dup;
	sort_list(dup);
	while (dup)
	{
		if (dup->value)
			printf("declare -x %s=\"%s\"\n", dup->key, dup->value);
		else
			printf("declare -x %s\n", dup->key);
		dup = dup->next;
	}
	free_list(tmp);
	return (0);
}

int	export_arg(t_node *node, t_env *env, int i, int sign)
{
	char	**tmp;
	char	*str;

	str = "not a valid identifier\n";
	while (node->argv[++i])
	{
		if (ft_strchr(node->argv[i], '='))
		{
			tmp = ft_split(node->argv[i], '=');
			if (check_arg_name(tmp[0], &sign))
				printf("minishell: export: `%s': %s", node->argv[i], str);
			else
				export_arg_equal(env, tmp);
			free_split(tmp);
		}
		else
		{
			if (check_arg_name(node->argv[i], &sign))
				printf("minishell: export: `%s': %s", node->argv[i], str);
			else
				export_arg_not_equal(env, node->argv[i]);
		}
	}
	return (sign);
}

void	export_arg_equal(t_env *env, char **tmp)
{
	if (!env)
		return ;
	while (env)
	{
		if (!ft_strcmp(env->key, tmp[0]))
		{
			if (env->value)
				free(env->value);
			env->value = ft_strtrim(tmp[1], "\"");
			return ;
		}
		if (env->next == NULL)
			break ;
		env = env->next;
	}
	env->next = (t_env *)malloc(sizeof(t_env));
	env = env->next;
	env->key = ft_strdup(tmp[0]);
	env->value = ft_strtrim(tmp[1], "\"");
	env->next = NULL;
}

void	export_arg_not_equal(t_env *env, char *arg)
{
	if (!env)
		return ;
	while (env)
	{
		if (!ft_strcmp(env->key, arg))
		{
			if (env->value)
				free(env->value);
			env->value = NULL;
			return ;
		}
		if (env->next == NULL)
			break ;
		env = env->next;
	}
	env->next = (t_env *)malloc(sizeof(t_env));
	env = env->next;
	env->key = ft_strdup(arg);
	env->value = NULL;
	env->next = NULL;
}
