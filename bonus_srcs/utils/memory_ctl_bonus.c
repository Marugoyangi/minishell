/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_ctl_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:21:21 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:21:21 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	free_env(t_env *env)
{
	t_env	*tmp;
	t_env	*tmp_env;

	tmp_env = env;
	while (tmp_env)
	{
		tmp = tmp_env->next;
		if (tmp_env->key)
			free(tmp_env->key);
		if (tmp_env->value)
			free(tmp_env->value);
		free(tmp_env);
		tmp_env = tmp;
	}
}

void	free_arg(t_arg *arg)
{
	free(arg->tilde);
	if (arg->error->token)
		free(arg->error->token);
	arg->error->token = NULL;
	free(arg->ps[0]);
	free(arg->ps[1]);
	free(arg->error);
	if (arg->envp_head)
		free_env(arg->envp_head);
	if (arg->minishell_path)
		free(arg->minishell_path);
}

void	free_ast(t_node *node)
{
	if (!node)
		return ;
	free_ast(node->right);
	free_ast(node->left);
	if (node->data && node->type != L_PIPELINE && \
	node->type != L_LOGICAL_OPERATOR)
		free(node->data);
	if (node->line)
	{
		if (node->line->data)
			free(node->line->data);
		if (node->line->info)
			free(node->line->info);
		free(node->line);
	}
	if (node->argv)
		free_split(node->argv);
	if (node->filename != NULL)
	{
		unlink(node->filename);
		free(node->filename);
		node->filename = NULL;
	}
	free(node);
	node = NULL;
}

void	free_node(t_node *node)
{
	if (!node)
		return ;
	free_node(node->right);
	if (node->data && node->type != L_PIPELINE && \
	node->type != L_LOGICAL_OPERATOR)
		free(node->data);
	if (node->line)
	{
		if (node->line->data)
			free(node->line->data);
		if (node->line->info)
			free(node->line->info);
		free(node->line);
	}
	if (node->argv)
		free_split(node->argv);
	if (node->filename != NULL)
	{
		unlink(node->filename);
		free(node->filename);
		node->filename = NULL;
	}
	free(node);
}

void	free_split(char **split)
{
	int		i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
