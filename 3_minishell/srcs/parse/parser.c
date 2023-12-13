/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:19:14 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/14 08:32:16 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_preorder(t_node *root)
{
	if (root == NULL)
		return ;
	if (root->data)
	printf("data: %s\n", root->data);
	printf("type: %d\n", root->type);
	if (root->argv)
	{
		int i = 0;
		printf("argv: ");
		while (root->argv[i])
		{
			printf("[%s]", root->argv[i]);
			i++;
		}
		printf("\n");
	}
	print_preorder(root->left);
	print_preorder(root->right);
}

t_node	*find_redirection(t_node *root)
{
	t_node	*tmp_cmd;
	t_node	*tmp_red;
	t_node	**reds;
	t_node	**cmds;

	if (root == NULL)
		return (NULL);
	if (root->type == L_SUBSHELL)
	{
		append_subshell(root);
		root->left = NULL;
		root->right = NULL;
		return (root);
	}
	tmp_cmd = append_cmd(root, L_SIMPLE_COMMAND);
	reds = sort_node(root, L_REDIRECTION);
	cmds = sort_node(root, L_SIMPLE_COMMAND);
	tmp_red = sort_redirection(reds);
	sort_free(cmds);
	free(reds);
	if (!tmp_cmd)
		return (tmp_red);
	else if (tmp_cmd && tmp_red)
	{	
		tmp_cmd->left = NULL;
		tmp_cmd->right = NULL;
		tmp_red->right = tmp_cmd;
		return (tmp_red);
	}
	return (tmp_cmd);
}

t_node	*find_pipe(t_node *root)
{
	t_node	*tmp;

	tmp = root;
	if (root == NULL)
		return (NULL);
	while (tmp)
	{
		if (tmp->type == L_PIPELINE)
		{
			if (tmp->left)
				tmp->left->right = NULL;
			if (tmp->right)
				tmp->right->left = NULL;
			tmp->left = find_pipe(tmp->left);
			tmp->right = find_redirection(tmp->right);
			return (tmp);
		}
		tmp = tmp->left;
	}
	tmp = root;
	while(tmp->left)
		tmp = tmp->left;
	return (find_redirection(tmp));
}

t_node	*find_operator(t_node *root)
{
	t_node	*tmp;

	tmp = root;
	if (root == NULL)
		return (NULL);
	while (tmp)
	{
		if (tmp->type == L_LOGICAL_OPERATOR)
		{
			if (tmp->right)
				tmp->right->left = NULL;
			if (tmp->left)
				tmp->left->right = NULL;
			tmp->left = find_pipe(tmp->left);
			tmp->right = find_operator(tmp->right);
			return (tmp);
		}
		tmp = tmp->right;
	}
	tmp = root;
	while (tmp->right)
		tmp = tmp->right;
	return (find_pipe(tmp));
}

void	parser(t_arg *arg)
{
	t_node	*root;

	root = find_operator(arg->ast_head);
	arg->ast_head = root;
	// print_preorder(arg->ast_head);
}