/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 10:54:38 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/17 10:55:59 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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

t_line	*get_line_info(char *env)
{
	t_line	*line;

	line = (t_line *)malloc(sizeof(t_line));
	line->data = env;
	line->info = NULL;
	tokenize(line, NULL);
	return (line);
}
