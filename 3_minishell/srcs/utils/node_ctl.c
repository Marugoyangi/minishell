/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_ctl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 03:23:22 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/12 22:02:30 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	printf("node length: %d\n", i);
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
	if (line == NULL)
		return (NULL);
	line->data = (char *)malloc(sizeof(char) * (node_data_length(node) + 1));
	if (line->data == NULL)
	{
		free(line);
		return (NULL);
	}
	line->info = (char *)malloc(sizeof(char) * (node_data_length(node) + 1));
	if (line->info == NULL)
	{
		free(line->data);
		free(line);
		return (NULL);
	}
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
	printf("real lenth is %d\n", i);
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

t_node	*create_node(char *data, t_line *line, int type)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (new_node == NULL)
		return (NULL);
	new_node->line = NULL;
	new_node->data = NULL;
	if (data)
		new_node->data = data;
	if (line)
		new_node->line = line;
	new_node->type = type;
	new_node->argv = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

// t_node	*create_ast_node(t_line *line, int start, int end, int type)
// {
// 	t_node	*new_node;

// 	if (start == end)
// 		start++;
// 	new_node = create_node(NULL, NULL, type);
// 	new_node->data = ft_substr(line->data, start, end - start);
// 	new_node->argv = NULL;
// 	new_node->type = type;
// 	return (new_node);
// }
