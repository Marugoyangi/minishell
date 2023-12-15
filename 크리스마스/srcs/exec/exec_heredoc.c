/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 02:02:43 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/16 02:32:46 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	get_heredoc_filename(t_node *root, int *i, t_arg *arg)
{
	t_node	*node;

	if (!root)
		return ;
	node = root;
	if (node->type == L_REDIRECTION)
	{
		if (node->data && !ft_strcmp(node->data, "<<"))
			node->filename = set_heredoc_filename(i);
	}
	if (arg->error->code == 2)
		get_heredoc_filename(node->right, i, arg);
	else
	{
		get_heredoc_filename(node->left, i, arg);
		get_heredoc_filename(node->right, i, arg);
	}
}
