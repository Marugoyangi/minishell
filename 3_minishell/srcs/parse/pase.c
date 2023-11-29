/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pase.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:19:14 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/11/29 20:30:09 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_node	*ft_find_operator(t_node *head)
{
	t_node	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == L_LOGICAL_OPERATOR)
			return (tmp);
		tmp = tmp->right;
	}
	return (NULL);
}


void	parser(t_arg *arg)
{
	t_node	*root;

	root = ft_find_operator(arg->ast_head);


}