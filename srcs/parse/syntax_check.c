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

void	check_logical_operator(t_node *node, t_error *error, int *found_type)
{
	t_node	*right;

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
	t_node	*right;

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
	int		i;

	i = 0;
	while (node->data[i] == '>' || node->data[i] == '<' || node->data[i] == ' ')
		i++;
	if (!node->argv && (node->data[i] == '\"' || node->data[i] == '\''))
		error->token = ft_strdup("\'");
	else if ((!node->argv && node->data[i] == '$'))
		error->token = ft_strdup(node->data + i);
	else if (node->argv && node->argv[1])
		error->token = ft_strdup("");
	else if (!node->argv && node->data[i] == '\0' && !node->right)
		error->token = ft_strdup("newline");
	else if (node->right && node->right->type == L_SUBSHELL)
		error->token = ft_strdup("(");
	else if (!node->argv && node->right)
		error->token = ft_strdup(node->right->data);
	else if ((node->right && (node->right->type == L_LOGICAL_OPERATOR || \
	node->right->type == L_REDIRECTION || node->right->type == L_PIPELINE || \
	node->right->type == L_SIMPLE_COMMAND)) || (node->left && \
	(node->left->type == L_SIMPLE_COMMAND || node->left->type == L_SUBSHELL)))
		return ;
	else if (node->right)
		error->token = ft_strdup(node->right->data);
}

int	check_syntax_token(t_arg *arg, int flag)
{
	if (arg->error->token)
	{
		if (flag == 1)
		{
			if (!ft_strcmp(arg->error->token, "newline"))
			{
				free(arg->error->token);
				arg->error->token = ft_strdup(")");
			}
			return (0);
		}
		else
			error_handler(arg, E_WRONG_SYNTAX);
		return (1);
	}
	return (0);
}

int	check_syntax(t_node *head, t_arg *arg, int flag)
{
	t_node	*tmp;
	int		found_type[3];

	tmp = head;
	ft_memset(found_type, 0, sizeof(found_type));
	arg->error->token = NULL;
	while (tmp && !arg->error->token)
	{
		if (tmp == head && tmp->type == L_LOGICAL_OPERATOR \
		&& tmp->right == NULL)
			arg->error->token = ft_strdup(tmp->data);
		else if (tmp->type == L_REDIRECTION)
			check_redirection(tmp, arg->error);
		else if (tmp->type == L_SIMPLE_COMMAND)
			check_command(tmp, arg->error, found_type);
		else if (tmp->type == L_SUBSHELL)
			check_subshell(tmp, arg->error, found_type, arg);
		else if (tmp->type == L_PIPELINE)
			check_pipeline(tmp, arg->error, found_type);
		else if (tmp->type == L_LOGICAL_OPERATOR)
			check_logical_operator(tmp, arg->error, found_type);
		tmp = tmp->right;
	}
	return (check_syntax_token(arg, flag));
}
