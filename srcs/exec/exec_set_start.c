/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_set_start.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:25:08 by seungwok          #+#    #+#             */
/*   Updated: 2024/01/10 10:18:45 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_exec(t_arg *arg)
{
	arg->fork_sign = 0;
	arg->last_exit_status = start_exec(arg->ast_head, arg);
	if (arg->last_exit_status)
		arg->error->code = arg->last_exit_status;
}

int	start_exec(t_node *node, t_arg *arg)
{
	if (!node)
		return (0);
	if (node->type == L_SUBSHELL)
		return (exec_subshell(node, arg));
	else if (node->type == L_LOGICAL_OPERATOR)
		return (exec_logical_operator(node, arg));
	else if (node->type == L_PIPELINE)
		return (exec_pipeline(node, arg));
	else if (node->type == L_REDIRECTION)
		return (exec_redirection(node, arg));
	else if (node->type == L_SIMPLE_COMMAND)
		return (exec_command(node, arg));
	else
		return (1);
}
