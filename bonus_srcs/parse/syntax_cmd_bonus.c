/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_cmd_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:23:21 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:23:21 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	check_double_command(int *found_type, int index)
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

int	syntax_subshell(t_node *node, t_arg *arg)
{
	t_arg	tmp;

	ft_memset(&tmp, 0, sizeof(t_arg));
	tmp.line.data = node->data;
	tmp.line.data = ft_strtrim(tmp.line.data, "()");
	if (tmp.line.data == NULL)
		return (1);
	tokenize(&tmp.line, NULL);
	lexicize(&tmp);
	if (!check_syntax(tmp.ast_head, arg, 1))
	{
		free(tmp.line.data);
		free(tmp.line.info);
		free_node(tmp.ast_head);
		return (1);
	}
	free(tmp.line.data);
	free(tmp.line.info);
	free_node(tmp.ast_head);
	return (0);
}

void	check_subshell(t_node *node, t_error *error, int *type, t_arg *arg)
{
	t_node	*right;

	if (check_double_command(type, 2))
	{
		error->token = ft_strdup("(");
		return ;
	}
	if (syntax_subshell(node, arg))
		return ;
	right = node->right;
	if (right == NULL)
		return ;
	if (right->type == L_SUBSHELL || (node->left && \
	(node->left->type == L_SIMPLE_COMMAND || \
	node->left->type == L_SUBSHELL || node->left->type == L_REDIRECTION)))
		error->token = ft_strdup("(");
	else if (right->type == L_LOGICAL_OPERATOR || right->type == L_REDIRECTION \
	|| right->type == L_PIPELINE)
		return ;
	else
		error->token = ft_strdup(right->data);
}

void	check_command(t_node *node, t_error *error, int *found_type)
{
	t_node	*right;

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
