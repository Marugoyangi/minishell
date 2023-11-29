/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_ctl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 03:23:22 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/11/29 20:14:33 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_node(char *data, int type, int *line_info_index) // 스페이스 넘기기 왜 필요하지?
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (new_node == NULL)
		return (NULL);
	new_node->type = type;
	new_node->data = data;
	new_node->left = NULL;
	new_node->right = NULL;
	*line_info_index = *line_info_index + ft_strlen(data);
	return (new_node);
}
