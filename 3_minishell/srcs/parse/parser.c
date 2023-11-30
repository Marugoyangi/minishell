/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:19:14 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/11/30 08:03:52 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_preorder(t_node *root)
{
	if (root == NULL)
		return ;
	printf("data: %s\n", root->data);
	printf("type: %d\n", root->type);
	print_preorder(root->left);
	print_preorder(root->right);
}

t_node	*find_redirection(t_node *root) //ls | grep "shell" > a && (ls | grepd "m" || ls | grep "src")
{
	t_node	*tmp;

	tmp = root;
	while (tmp->right)
		tmp = tmp->right;
	if (tmp->type == L_REDIRECTION)
	{
		if (tmp->left)
		tmp->left->right = NULL;
		tmp->left = find_redirection(tmp->left);
		return (tmp);
	}
	return (root); // 명령어 1개만 있는 경우
}

t_node	*find_pipe(t_node *root)
{
	t_node	*tmp;

	tmp = root;
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
	while (tmp->left)
		tmp = tmp->left; // 마지막 노드로 이동
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
		tmp = tmp->right; // 마지막 노드로 이동
	return (find_pipe(tmp));
}

void	parser(t_arg *arg)
{
	t_node	*root;

	root = find_operator(arg->ast_head);
	arg->ast_head = root;
	printf("-------------------------\n");
	print_preorder(arg->ast_head);
}
