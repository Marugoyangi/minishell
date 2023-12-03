/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_ctl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 03:23:22 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/03 08:01:26 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*find_envp_value(t_node *envp_head, char *key)
// {
// 	t_node	*tmp;
// 	char	*value;

// 	tmp = envp_head;
// 	while (tmp)
// 	{
// 		if (ft_strcmp(tmp->data, key) == 0)
// 		{
// 			value = tmp->type;
// 			return (value);
// 		}
// 		tmp = tmp->right;
// 	}
// 	return (NULL);
// }

t_node *last_node(t_node *head)
{
	t_node *tmp;

if (head == NULL)
		return (NULL);
	tmp = head;
	while (tmp->right)
		tmp = tmp->right;
	return (tmp);
}

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
	if (line_info_index)
		*line_info_index = *line_info_index + ft_strlen(data);
	return (new_node);
}
