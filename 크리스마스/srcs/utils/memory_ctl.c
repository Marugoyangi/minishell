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

void	sort_free(t_node **node)
{
	int	i;

	i = 0;
	if (node == NULL)
		return ;
	printf("is it HERE?\n");
	while (node[i])
	{
		if (node[i]->data)
			free(node[i]->data);
		if (node[i]->argv)
			free_split(node[i]->argv);
		if (node[i]->line)
		{
			free(node[i]->line->data);
			free(node[i]->line->info);
			free(node[i]->line);
		}
		free(node[i]);
		i++;
	}
	free (node);
}

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
	free(arg->error);
	if (arg->envp_head)
		free_env(arg->envp_head);
	if (arg->pwd)
		free(arg->pwd);
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
	printf("is it here now ? AST\n");
	free(node);
	node = NULL;
}

void	free_node(t_node *node)
{
	t_node	*tmp;

	if (!node)
		return ;
	while (node)
	{
		tmp = node->right;
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
		free(node);
		node = tmp;
	}
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
