/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 21:49:59 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/02 21:49:59 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	expand_asterisk(t_line **line)
{
	t_node	**result;
	int		i;
	int		start;
	int		is_asterisk;

	is_asterisk = 0;
	i = 0;
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
		{
			char **p = line_split((*line)->data, '/');
			result = filter_asterisk(p);
			free_split((void **)p);
			if (*result == NULL)
				make_line_back((*line), start, i);
			else
			{
				t_line *filtered_line = node_to_line(*result);
				printf("filtered_line->data: %s\n", filtered_line->data);
				is_asterisk = replace_line(filtered_line, line, start, i);
				printf("line->data: %s\n", (*line)->data);
				i = start + is_asterisk;
				free(filtered_line->data);
				free(filtered_line->info);
				free(filtered_line);
				while (*result)
				{
					t_node *tmp;

					tmp = (*result)->right;
					free((*result)->data);
					free(*result);
					*result = tmp;
				}
				free(result);
			}
			is_asterisk = 0;
		}
	}
}
