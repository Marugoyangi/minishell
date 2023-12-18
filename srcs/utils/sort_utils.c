/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 13:50:01 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/19 00:55:32 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	node_type_numbers(t_node *root, int type)
{
	t_node	*tmp;
	int		i;
	int		j;

	if (root == NULL)
		return (0);
	tmp = root;
	i = 0;
	while (tmp)
	{
		if (tmp->type == type && tmp->type == L_SIMPLE_COMMAND && \
		tmp->argv)
		{
			j = 0;
			while (tmp->argv && tmp->argv[j])
				j++;
			i += j;
		}
		else if (tmp->type == type)
			i++;
		tmp = tmp->right;
	}
	return (i);
}

void	free_line(t_line *line)
{
	if (line == NULL)
		return ;
	if (line->data)
		free(line->data);
	if (line->info)
		free(line->info);
	free(line);
}

void	sort_free(t_node **node)
{
	int	i;

	i = 0;
	if (node == NULL)
		return ;
	while (node[i])
	{
		if (node[i]->data)
			free(node[i]->data);
		if (node[i]->argv)
			free_split(node[i]->argv);
		if (node[i]->line)
		{
			free(node[i]->line->data);
			free(node[i]->line->info);
			free(node[i]->line);
		}
		free(node[i]);
		i++;
	}
	free (node);
}
