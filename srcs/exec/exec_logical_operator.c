/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_logical_operator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 18:54:16 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/15 12:43:24 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		exec_logical_operator(t_node *node, t_arg *arg);

int	exec_logical_operator(t_node *node, t_arg *arg)
{
	if (!strcmp(node->data, "&&"))
	{
		if (!start_exec(node->left, arg))
		{
			if (!start_exec(node->right, arg))
				return (0);
			else
				return (1);
		}
		else
			return (1);
	}
	else if (!strcmp(node->data, "||"))
	{
		if (start_exec(node->left, arg))
		{
			if (!start_exec(node->right, arg))
				return (0);
			else
				return (1);
		}
		else
			return (0);
	}
	return (1);
}
