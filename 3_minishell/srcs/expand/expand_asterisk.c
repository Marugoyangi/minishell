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

void	expand_asterisk(int *position, t_line *line)
{
	t_node	**result;
	int		i;
	int		start;
	int		is_asterisk;

	i = position[0];
	is_asterisk = 0;
	while (i < position[1] && line->info[i])
	{
		while (line->info[i] == T_SPACE)
			i++;
		start = i;
		while (i < position[1] && line->info[i] && line->info[i] != T_SPACE)
		{
			if (line->info[i] == T_ASTERISK)
			{
				line->data[i] = -T_ASTERISK;
				is_asterisk = 1;
			}
			i++;
		}
		if (is_asterisk == 1)
		{
			result = filter_asterisk(line_split(ft_substr(line->data, start,  i - start), '/'));
			if (*result == NULL)
				make_line_back(line, start, i);
			else
			{
				is_asterisk = add_line(node_to_line(*result), line, start, i);
				position[1] += is_asterisk - 1;
				i = start + is_asterisk;
			}
			is_asterisk = 0;
		}
	}
}
