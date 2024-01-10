/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:07:36 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:22:37 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	remove_quotes(t_line **line)
{
	int		i;
	int		start;

	i = 0;
	while ((*line) && (*line)->data && (*line)->info && (*line)->info[i])
	{
		while ((*line)->info[i] && (*line)->info[i] != T_SINGLE_QUOTE && \
		(*line)->info[i] != T_DOUBLE_QUOTE)
			i++;
		start = i;
		while ((*line) && (*line)->info && (*line)->info[i] && \
		((*line)->info[i] == T_SINGLE_QUOTE || (*line)->info[i] == \
		T_DOUBLE_QUOTE))
		{
			i++;
			ft_delete_line(i - start, line, i, 0);
			i -= i - start;
		}
	}
}

int	is_number(char c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}

int	is_not_delimiter(char c)
{
	if (('a' <= c && c <= 'z') || c == '_' || ('A' <= c && c <= 'Z'))
		return (1);
	return (0);
}

void	tokenize_literal(t_line *line)
{
	int	i;

	i = 0;
	line->info = ft_strdup(line->data);
	while (line->info[i])
	{
		line->info[i] = T_LITERAL;
		i++;
	}
}

t_line	*get_info(char *env, int flag)
{
	t_line	*line;

	line = (t_line *)malloc(sizeof(t_line));
	line->data = env;
	line->info = NULL;
	if (flag == 0)
		tokenize(line, NULL);
	else
		tokenize_literal(line);
	return (line);
}
