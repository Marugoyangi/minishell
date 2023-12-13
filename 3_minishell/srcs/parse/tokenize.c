/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:49:00 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/11 15:31:38 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	check_ampersand(char c)
{
	if (c != '&')
		return (T_WORD);
	else
		return (T_OPERATOR);
}

char	check_operator(char c, int *quote)
{
	if (c == '|' && *quote == 0)
		return (T_OPERATOR);
	else if (c == '&' && *quote == 0)
		return (-1);
	else if (c == '<' && *quote == 0)
		return (T_OPERATOR);
	else if (c == '>' && *quote == 0)
		return (T_OPERATOR);
	else
		return (T_WORD);
}

char	check_literal(char c, int *quote, int *subshell)
{
	if (c == '*' && *quote == 0)
		return (T_ASTERISK);
	else if (c == '(' && *quote == 0 && *subshell == 0)
	{
		*subshell = 1;
		return (T_PARENTHESIS);
	}
	else if (c == ')' && *quote == 0 && *subshell == 1)
	{
		*subshell = 0;
		return (T_PARENTHESIS);
	}
	else if (c == ')' && *quote == 0 && *subshell == 0)
	{
		*subshell = -1;
		return (-1);
	}
	else if ((c == ' ' || c == '\t') && *quote == 0)
		return (T_SPACE);
	else if (c && (*quote == T_SINGLE_QUOTE || *quote == T_DOUBLE_QUOTE))
		return (T_LITERAL);
	else
		return (-1);
}

char	check_quote(char c, int *quote)
{
	if (c == '\'' && *quote == 0)
	{
		*quote = T_SINGLE_QUOTE;
		return (T_SINGLE_QUOTE);
	}
	else if (c == '\"' && *quote == 0)
	{
		*quote = T_DOUBLE_QUOTE;
		return (T_DOUBLE_QUOTE);
	}
	else if (c == '\'' && *quote == T_SINGLE_QUOTE)
	{
		*quote = 0;
		return (T_SINGLE_QUOTE);
	}
	else if (c == '\"' && *quote == T_DOUBLE_QUOTE)
	{
		*quote = 0;
		return (T_DOUBLE_QUOTE);
	}
	else if (c == '$' && *quote != T_SINGLE_QUOTE)
		return (T_ENV);
	return (-1);
}

void	tokenize(t_line *line, t_arg *arg)
{
	int		quote;
	int		subshell;
	int		i;

	quote = 0;
	subshell = 0;
	i = 0;
	if (line->data[0] == '\0')
		return ;
	line->info = ft_strdup(line->data);
	while (line->data[i])
	{
		line->info[i] = check_quote(line->data[i], &quote);
		if (line->info[i] == -1)
			line->info[i] = check_literal(line->data[i], &quote, &subshell);
		if (line->info[i] == -1)
			line->info[i] = check_operator(line->data[i], &quote);
		if (line->info[i] == -1)
			line->info[i] = check_ampersand(line->data[i + 1]);
		i++;
	}
	line->info[i] = '\0';
	if ((quote != 0 || subshell == -1 || subshell == 1 ) && arg)
	{
		arg->error->type = E_UNCLOSED_QUOTES;
		error_handler(arg);
	}
	printf("line : %s\n", line->data);
	printf("new_line : ");
	i = 0;
	while (line->info[i])
	{
		printf("[%d]", line->info[i]);
		i++;
	}
	printf("\n");
}
