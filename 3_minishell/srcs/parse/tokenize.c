/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:49:00 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/03 02:10:06 by jeongbpa         ###   ########.fr       */
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
	else if (c == '$' && *quote == 0)
		return (T_ENV);
	else if (c == '$' && *quote == 1)
		return (T_ENV_QUOTED);
	return (-1);
}

void	check_line(t_arg *arg)
{
	int		quote;
	int		subshell;
	int		i;

	quote = 0;
	subshell = 0;
	i = 0;
	if (arg->input.data[0] == '\0')
		return ;
	arg->input.data = ft_strtrim(arg->input.data, " \t");
	arg->input.info = ft_strdup(arg->input.data);
	if (arg->input.info == NULL)
		return ;
	while (arg->input.data[i])
	{
		arg->input.info[i] = check_quote(arg->input.data[i], &quote);
		if (arg->input.info[i] == -1)
			arg->input.info[i] = check_literal(arg->input.data[i], &quote, &subshell);
		if (arg->input.info[i] == -1)
			arg->input.info[i] = check_operator(arg->input.data[i], &quote);
		if (arg->input.info[i] == -1)
			arg->input.info[i] = check_ampersand(arg->input.data[i + 1]);
		i++;
	}
	if (quote != 0 || subshell == -1 || subshell == 1)
	{
		printf("quote : %d\n", quote);
		printf("subshell : %d\n", subshell);
		printf("syntax error\n");
		return ;
	}
	printf("line : %s\n", arg->input.data);
	printf("new_line : ");
	i = 0;
	while (arg->input.info[i])
	{
		printf("[%d]", arg->input.info[i]);
		i++;
	}
	printf("\n");
}
