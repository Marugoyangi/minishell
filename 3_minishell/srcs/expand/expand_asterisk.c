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

void	expand_asterisk(int *position, t_line *line)
{
	t_node	*result;
	int		i;
	int		start;
	int		is_asterisk;

	i = position[0];
	is_asterisk = 0;
	result = NULL;
	while (line->info[i])
	{
		while (line->info[i] == T_SPACE)
			i++;
		while (line->info[i] && line->info[i] != T_SPACE)
		{
			start = i;
			if (line->info[i] == T_ASTERISK)
			{
				line->data[i] = -T_ASTERISK;
				is_asterisk = 1;
			}
			i++;
		}
		if (line->info[i] && line->info[i] == T_SPACE)
		{
			if (is_asterisk == 1)
			{
				result = filter_asterisk(line_split(ft_substr(line->data, start,  i - start), '/'));
				make_it_normal(line, start, i);
				while (result)
				{
					if (result->right == NULL)
						i += replace_line(result->data, line, start, i);
					else
						i += replace_line(ft_strjoin(result->data, " "), line, start, i);
					result = result->right;
				}
				is_asterisk = 0;
			}
		}
	}
	position[1] = i;
}
