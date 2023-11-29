/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_BNF.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 02:27:02 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/11/27 02:28:16 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_BNF(char *new_line)
{
	int		i;
	int		quote;
	int		subshell;
	char	type;

	i = 0;
	quote = 0;
	subshell = 0;
	while (new_line[i])
	{
		type = check_literal(new_line[i], &quote, &subshell);
		if (type == -1)
			return (-1);
		else if (type == OPERATOR)
		{
			if (check_operator(new_line[i], &quote, &subshell) == -1)
				return (-1);
		}
		else if (type == PARENTHESIS)
		{
			if (check_parenthesis(new_line[i], &quote, &subshell) == -1)
				return (-1);
		}
		else if (type == LITERAL)
		{
			if (check_literal(new_line[i], &quote, &subshell) == -1)
				return (-1);
		}
		else if (type == _SPACE)
		{
			if (check_space(new_line[i], &quote, &subshell) == -1)
				return (-1);
		}
		i++;
	}
	if (quote != 0 || subshell != 0)
		return (-1);
	return (0);
}