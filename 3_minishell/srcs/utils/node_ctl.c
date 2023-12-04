/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_ctl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 03:23:22 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/05 03:22:36 by jeongbpa         ###   ########.fr       */
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

int	node_data_length(t_node *node)
{
	int	i;
	t_node	*tmp;

	i = 0;
	tmp = node;
	while (tmp)
	{
		i += ft_strlen(tmp->data);
		if (tmp->right)
			i++;
		tmp = tmp->right;
	}
	printf("how much? %d\n", i);
	return (i);
}

t_line *node_to_line(t_node *node)
{
	int		i;
	int		j;
	t_line	*line;
	t_node	*tmp;

	i = 0;
	j = 0;
	line = (t_line *)malloc(sizeof(t_line));
	line->data = (char *)malloc(sizeof(char) * (node_data_length(node) + 1));
	line->info = (char *)malloc(sizeof(char) * (node_data_length(node) + 1));
	tmp = node;
	while (tmp)
	{
		j = 0;
		while (tmp->data[j])
		{
			line->data[i] = tmp->data[j];
			line->info[i] = T_LITERAL;
			i++;
			j++;
		}
		if (tmp->right)
		{
			line->data[i] = ' ';
			line->info[i] = T_SPACE;
			i++;
		}
		tmp = tmp->right;
	}
	line->data[i] = '\0';
	line->info[i] = '\0';
	return (line);
}

char	**node_to_data(t_node *node)
{
	int		i;
	char	**line;

	i = 0;
	line = (char **)malloc(sizeof(char *) * (node_data_length(node) + 1));
	if (line == NULL)
		return (NULL);
	while (node)
	{
		line[i] = ft_strdup(node->data);
		i++;
		node = node->right;
	}
	line[i] = NULL;
	return (line);
}

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
