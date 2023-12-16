/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 09:20:12 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/06 09:20:12 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_quote(t_arg *arg)
{
	write (2, "minishell: syntax error either unclosed quote ", 46);
	write (2, "or parenthesis found\n", 21);
	arg->error->code = 2;
	if (arg->is_subshell)
		exit(arg->error->code);
}

void	error_syntax(t_arg *arg)
{
	if (arg->is_subshell)
		exit(arg->error->code);
	if (arg->error->token[0] == '$')
	{
		write(2, "minishell: ", 11);
		write(2, arg->error->token, ft_strlen(arg->error->token));
		write(2, ": ambiguous redirect\n", 21);
	}
	else if (arg->error->token[0] == '\'')
		write(2, "minishell: No such file or directory\n", 38);
	else if (arg->error->token[0] == '\0')
		write(2, "minishell: ambiguous redirect\n", 31);
	else
	{
		write(2, "minishell: syntax error near unexpected token `", 47);
		write(2, arg->error->token, ft_strlen(arg->error->token));
		write(2, "\'\n", 2);
	}
	arg->error->code = 2;
}

void	error_handler(t_arg *arg, int type)
{
	if (type == E_WRONG_SYNTAX)
		error_syntax(arg);
	else if (type == E_UNCLOSED_QUOTES)
		error_quote(arg);
}
