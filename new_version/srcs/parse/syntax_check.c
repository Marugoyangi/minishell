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

int		check_double_command(int *found_type, int index)
{
	int	another;

	if (index == 1)
		another = 2;
	else if (index == 2)
		another = 1;
	if (found_type[0] == 0)
	{
		found_type[index] = 2;
		found_type[another] = 1;
		found_type[0] = 1;
		return (0);
	}
	else if (found_type[index] > 1 && found_type[another] > 1)
		return (1);
	else
		found_type[index]++;
	return (0);
}

void	check_logical_operator(t_node *node, t_error *error, int *found_type)
{
	t_node *right;

	right = node->right;
	found_type[0] = 0;
	if (right == NULL)
		error->token = ft_strdup("newline");
	else if (right->type == L_LOGICAL_OPERATOR || right->type == L_PIPELINE)
		error->token = ft_strdup(right->data);
	else
		return ;
}

void	check_pipeline(t_node *node, t_error *error, int *found_type)
{
	t_node *right;

	right = node->right;
	found_type[0] = 0;
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
	if (!node->argv && !node->right)
		error->token = ft_strdup("newline");
	else if (right && right->type == L_SUBSHELL)
		error->token = ft_strdup("(");
	else if (!node->argv)
		error->token = ft_strdup(right->data);
	else if ((right && (right->type == L_LOGICAL_OPERATOR || right->type == L_REDIRECTION \
	|| right->type == L_PIPELINE || right->type == L_SIMPLE_COMMAND )) || \
	(node->left && (node->left->type == L_SIMPLE_COMMAND || node->left->type == L_SUBSHELL)))
		return ;
	else if (right)
		error->token = ft_strdup(right->data);
}

void	check_subshell(t_node *node, t_error *error, t_arg *arg, int *found_type)
{
	t_node *right;

	if (check_double_command(found_type, 2))
	{
		error->token = ft_strdup("(");
		return ;
	}
	if (syntax_subshell(node, arg))
		return ;
	right = node->right;
	if (right == NULL)
		return ;
	if (right->type == L_SUBSHELL ||  \
	(node->left && (node->left->type == L_SIMPLE_COMMAND || node->left->type == L_SUBSHELL \
	|| node->left->type == L_REDIRECTION)))
		error->token = ft_strdup("(");
	else if (right->type == L_LOGICAL_OPERATOR || right->type == L_REDIRECTION \
	|| right->type == L_PIPELINE)
		return ;
	else 
		error->token = ft_strdup(right->data);
}

void	check_command(t_node *node, t_error *error, int *found_type)
{
	t_node *right;

	right = node->right;
	if (check_double_command(found_type, 1))
	{
		error->token = ft_strdup(node->data);
		return ;
	}
	if (right == NULL)
		return ;
	if (right->type == L_LOGICAL_OPERATOR || right->type == L_REDIRECTION \
	|| right->type == L_PIPELINE || right->type == L_SIMPLE_COMMAND)
		return ;
	else if (node->left && node->left->type == L_SUBSHELL)
		error->token = ft_strdup(node->data);
	else if (right->type == L_SUBSHELL)
		error->token = ft_strdup("(");
	else
		error->token = ft_strdup(right->data);
}

int check_syntax(t_node *head, t_arg *arg, int flag)
{
	// 리다이렉션이 단독으로 존재가능한지 클러에서 확인
	t_node	*tmp;
	int		found_type[3];

	tmp = head;
	memset(found_type, 0, sizeof(found_type));
	arg->error->token = NULL;
	while (tmp)
	{
		if (tmp == head && tmp->type == L_LOGICAL_OPERATOR && tmp->right == NULL)
			arg->error->token = ft_strdup(tmp->data);
		else if (tmp->type == L_REDIRECTION)
			check_redirection(tmp, arg->error);
		else if (tmp->type == L_SIMPLE_COMMAND)
			check_command(tmp, arg->error, found_type);
		else if (tmp->type == L_SUBSHELL)
			check_subshell(tmp, arg->error, arg, found_type);
		else if (tmp->type == L_PIPELINE)
			check_pipeline(tmp, arg->error, found_type);
		else if (tmp->type == L_LOGICAL_OPERATOR)
			check_logical_operator(tmp, arg->error, found_type);
		if (arg->error->token)
			break ;	
		tmp = tmp->right;
	}
	if (arg->error->token)
	{
		if (flag == 1 && !ft_strcmp(arg->error->token, "newline"))
		{
			ft_free((void *)arg->error->token);
			arg->error->token = ft_strdup(")");
		}
		arg->error->type = E_WRONG_SYNTAX;
		arg->error->code = 2;
		error_handler(arg);
		return (1);
	}
	return (0);
}