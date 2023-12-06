/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_ctl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 03:23:22 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/06 22:50:37 by jeongbpa         ###   ########.fr       */
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

t_node	*create_node(char *data, int type, int *i)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (new_node == NULL)
		return (NULL);
	new_node->type = type;
	new_node->data = data;
	new_node->left = NULL;
	new_node->right = NULL;
	if (i == NULL)
		return (new_node);
	*i = *i + ft_strlen(data);
	return (new_node);
}

char **detatch_command(char **argv)
{
	int		i;
	int		j;
	char	**new_argv;

	i = 0;
	while (argv[i])
		i++;
	new_argv = (char **)malloc(sizeof(char *) * (i + 1));
	j = 1;
	while (argv[j])
	{
		new_argv[j - 1] = ft_strdup(argv[j]);
		free(argv[j]);
		j++;
	}
	free(argv[0]);
	free(argv);
	return (new_argv);
}

t_node	*create_ast_node(t_line *line, int start, int end, int type)
{
	t_node	*new_node;

	if (start == end)
		start++;
	new_node = create_node(NULL, type, NULL);
	// new_node->argv = line_split(line->info, T_SPACE);
	// printf("new_node->argv[2] : %s\n", new_node->argv[0]);
	new_node->data = ft_substr(line->data, start, end - start);
	printf("new_node->data : %s\n", new_node->data);
	// new_node->argv = detatch_command(new_node->argv);
	new_node->type = type;
	return (new_node);
}
