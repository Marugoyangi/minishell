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

void	error_handler(t_arg *arg, int type)
{
	if (type == E_WRONG_SYNTAX)
	{
		if (arg->error->token[0] == '$')
			printf("minishell: %s: ambiguous redirect\n", arg->error->token);
		else if (arg->error->token[0] == '\'')
			printf("minishell: No such file or directory\n");
		else if (arg->error->token[0] == '\0')
			printf("minishell: ambiguous redirect\n");
		else
			printf("minishell: syntax error near unexpected token `%s'\n", \
			arg->error->token);
		arg->error->code = 2;
		printf("error code: %d\n", arg->error->code);
		if (arg->is_subshell)
			exit(arg->error->code);
	}
	else if (type == E_UNCLOSED_QUOTES)
	{
		write (2, "minishell: syntax error either unclosed quotes or parenthesis found\n", 69);
		arg->error->code = 2;
		if (arg->is_subshell)
			exit(arg->error->code);
	}
}
