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
	if (arg->error.type == E_MALLOC)
	{
		printf("minishell: memory allocation error\n");
		exit(1);
	}
	else if (arg->error.type == E_WRONG_SYTAX)
	{
		printf("minishell: syntax error near unexpected token `%s'\n", arg->error.token);
		exit(2);
	}
	else if (arg->error.type == E_FD)
	{
		write (2, "minishell: invalid file descriptor\n", 35);
		exit(1);
	}
	#고치기
	
}