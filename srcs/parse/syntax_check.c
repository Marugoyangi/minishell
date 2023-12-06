/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 09:45:58 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/06 09:45:58 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_logical_operator(t_node *node, t_error *error)
{
	t_node *right;

	right = node->right;
	if (right == NULL)
		error->token = ft_strdup("newline");
	else if (right->type == L_LOGICAL_OPERATOR || right->type == L_PIPELINE)
		error->token = ft_strdup(right->data);
	else
		return ;
}

void	check_pipeline(t_node *node, t_error *error)
{
	t_node *right;

	right = node->right;
	if (right == NULL)
		error->token = ft_strdup("newline");
	else if (right->type == L_LOGICAL_OPERATOR || right->type == L_PIPELINE)
		error->token = ft_strdup(right->data);
	else
		return ;
}

void	check_redirection(t_node *node, t_error *error)
{
	t_node *right;

	right = node->right;
	if (right == NULL)
		return ;
	// if (!node->argv[0])
	// 	error->token = ft_strdup(node->data);
	else if (right->type == L_LOGICAL_OPERATOR || right->type == L_REDIRECTION \
	|| right->type == L_PIPELINE)
		return ;
	else
		error->token = ft_strdup(right->data);
}

void	check_command(t_node *node, t_error *error, t_arg *arg)
{
	t_node *right;

	if (node->type == L_SUBSHELL)
		syntax_subshell(node, arg);
	right = node->right;
	if (right == NULL)
		return ;
	if (right->type == L_LOGICAL_OPERATOR || right->type == L_REDIRECTION \
	|| right->type == L_PIPELINE)
		return ;
	else if (right->type == L_SUBSHELL)
		error->token = ft_strdup("(");
	else
		error->token = ft_strdup(right->data);
}

void		check_syntax(t_node *head, t_arg *arg, int flag)
{
	// 리다이렉션이 단독으로 존재가능한지 클러에서 확인
	t_node	*tmp;

	tmp = head;
	arg->error->token = NULL;
	while (tmp)
	{
		if (tmp == head && tmp->type == L_LOGICAL_OPERATOR && tmp->right == NULL)
			arg->error->token = ft_strdup(tmp->data);
		else if (tmp->type == L_REDIRECTION && !tmp->argv && !tmp->right)
			arg->error->token = ft_strdup("newline");
		else if (tmp->type == L_SIMPLE_COMMAND || tmp->type == L_SUBSHELL)
			check_command(tmp, arg->error, arg);
		else if (tmp->type == L_PIPELINE)
			check_pipeline(tmp, arg->error);
		else if (tmp->type == L_LOGICAL_OPERATOR)
			check_logical_operator(tmp, arg->error);
		else if (tmp->type == L_REDIRECTION)
			check_redirection(tmp, arg->error);
		if (arg->error->token)
			break ;
		tmp = tmp->right;
	}
	if (arg->error->token)
	{
		if (flag == 1 && !ft_strcmp(arg->error->token, "newline"))
			arg->error->token = ft_strdup(")");
		arg->error->type = E_WRONG_SYNTAX;
		arg->error->code = 2;
		error_handler(arg);
	}
}