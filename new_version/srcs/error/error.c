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

void	error_handler(t_arg *arg)
{
	if (arg->error->type == E_MALLOC)
	{
		printf("minishell: memory allocation error\n");
	}
	else if (arg->error->type == E_WRONG_SYNTAX)
	{
		if (arg->error->token[0] == '$')
			printf("minishell: %s: ambiguous redirect\n", arg->error->token);
		else if (arg->error->token[0] == '\'')
			printf("minishell: No such file or directory\n");
		else if (arg->error->token[0] == '\0')
			printf("minishell: ambiguous redirect\n");
		else
			printf("minishell: syntax error near unexpected token `%s'\n", arg->error->token);
		free_node(arg->ast_head);
		arg->ast_head = NULL;
		ft_free((void *)arg->line.data);
		arg->line.data = NULL;
		ft_free((void *)arg->line.info);
		arg->line.info = NULL;

	}
	else if (arg->error->type == E_FD)
	{
		write (2, "minishell: invalid file descriptor\n", 35);
	}
	else if (arg->error->type == E_UNCLOSED_QUOTES)
	{
		write (2, "minishell: syntax error either unclosed quotes or parenthesis found\n", 69);
		exit (2);
	}
}
