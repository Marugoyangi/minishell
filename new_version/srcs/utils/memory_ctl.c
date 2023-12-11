/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_ctl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 23:03:57 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/04 23:03:57 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;
	t_env	*tmp_env;

	tmp_env = env;
	while (tmp_env)
	{
		tmp = tmp_env->next;
		if (tmp_env->key)
			ft_free((void *)tmp_env->key);
		if (tmp_env->value)
			ft_free((void *)tmp_env->value);
		free(tmp_env);
		tmp_env = tmp;
	}
}

void	free_arg(t_arg *arg)
{
	ft_free((void *)arg->tilde);
	ft_free((void *)arg->error->token);
	ft_free((void *)arg->error);
	if (arg->envp_head)
		free_env(arg->envp_head);
}

void	free_ast(t_node *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->data && node->type != L_PIPELINE && node->type && node->type != L_LOGICAL_OPERATOR)
		ft_free((void *)node->data);
	if (node->line)
	{
		ft_free((void *)node->line->data);
		ft_free((void *)node->line->info);
	}
	if (node->line)
		free(node->line);
	if (node->argv)
		free_split((void **)node->argv);
	free(node);
}

void	free_node(t_node *node)
{
	t_node	*tmp;

	while (node)
	{
		tmp = node->right;
		if (node->data)
			free(node->data);
		if (node->line)
		{
			if (node->line->data)
				free(node->line->data);
			if (node->line->info)
				free(node->line->info);
		}
		free(node->line);
		if (node->argv)
			free_split((void **)node->argv);
		ft_free((void *)node);
		node = tmp;
	}
}

void	free_split(void **split)
{
	int		i;

	i = 0;
	if (split == NULL)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	*ft_malloc(int size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		//error_handler("malloc error");
		exit(1);
	}
	return (ptr);
}

void	ft_free(void *ptr)
{
	if (ptr)
		free(ptr);
}
