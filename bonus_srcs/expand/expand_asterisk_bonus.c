/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:25:21 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:25:21 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	make_line_back(t_line *line, int start, int end)
{
	int	i;

	i = start;
	while (i < end)
	{
		if (line->info[i] == T_ASTERISK)
		{
			line->data[i] = '*';
			line->info[i] = T_LITERAL;
		}
		i++;
	}
}

int	asterisk_to_line(t_line **line, int start, int *end, int index)
{
	t_node	**result;
	t_line	*filtered_line;
	t_node	*tmp;
	char	**p;

	p = line_split((*line)->data + index, '/');
	result = filter_asterisk(p);
	free_split(p);
	if (*result == NULL)
		make_line_back((*line), start, *end);
	else
	{
		filtered_line = node_to_line(*result);
		*end = start + replace_line(filtered_line, line, start, *end);
		while (*result)
		{
			tmp = (*result)->right;
			free((*result)->data);
			free(*result);
			*result = tmp;
		}
	}
	free(result);
	return (0);
}

void	expand_asterisk(t_line **line, int index)
{
	int		i;
	int		start;
	int		is_asterisk;

	is_asterisk = 0;
	i = index;
	while ((*line) && (*line)->info && (*line)->info[i])
	{
		while ((*line)->info[i] == T_SPACE)
			i++;
		start = i;
		while ((*line)->info[i] && (*line)->info[i] != T_SPACE)
		{
			if ((*line)->info[i] == T_ASTERISK)
			{
				(*line)->data[i] = -T_ASTERISK;
				is_asterisk = 1;
			}
			i++;
		}
		if (is_asterisk == 1)
			is_asterisk = asterisk_to_line(line, start, &i, index);
	}
}
