/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_ctl_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 03:23:22 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:21:42 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	node_data_length(t_node *node)
{
	int		i;
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
	return (i);
}

t_line	*create_line(int size)
{
	t_line	*line;

	line = (t_line *)malloc(sizeof(t_line));
	line->data = (char *)malloc(sizeof(char) * (size + 1));
	line->info = (char *)malloc(sizeof(char) * (size + 1));
	line->data[size] = '\0';
	line->info[size] = '\0';
	return (line);
}

t_line	*node_to_line(t_node *node)
{
	int		i;
	int		j;
	t_line	*line;
	t_node	*tmp;

	i = 0;
	j = 0;
	tmp = node;
	line = create_line(node_data_length(node));
	while (tmp)
	{
		j = -1;
		while (tmp->data[++j])
		{
			line->data[i] = tmp->data[j];
			line->info[i++] = T_LITERAL;
		}
		if (tmp->right)
		{
			line->data[i] = ' ';
			line->info[i++] = T_SPACE;
		}
		tmp = tmp->right;
	}
	return (line);
}

t_node	*last_node(t_node *head)
{
	t_node	*tmp;

	if (head == NULL)
		return (NULL);
	tmp = head;
	while (tmp->right)
		tmp = tmp->right;
	return (tmp);
}

t_node	*create_node(char *data, t_line *line, int type)
{
	t_node	*new_node;

	new_node = (t_node *)ft_malloc(sizeof(t_node));
	if (data)
		new_node->data = data;
	else
		new_node->data = NULL;
	if (line)
		new_node->line = line;
	else
		new_node->line = NULL;
	new_node->filename = NULL;
	new_node->type = type;
	new_node->argv = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}
