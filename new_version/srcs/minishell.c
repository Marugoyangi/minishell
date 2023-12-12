/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:37 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/12 23:05:17 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_arg 			arg;
	struct termios	term;
	struct termios	original_term;
	
	(void )argc;
	(void )argv;
	terminal_init(&arg, &term, &original_term, envp);
	terminal_interactive(&term);
	print_ascii();
	while (1)
	{
		arg.line.data = readline(get_ps1(&arg));
		if (!arg.line.data)
		{
			terminal_default(&original_term, 1, &arg);
			break ;
		}
		add_history(arg.line.data);
		arg.line.data = modified_strtrim(arg.line.data, " \t\n");
		tokenize(&arg.line, &arg);
		lexicize(&arg);
		expand_vars(&arg);
		if (check_syntax(arg.ast_head, &arg, 0))
			continue ; 
		parser(&arg);
		terminal_default(&original_term, 0, &arg);
		set_exec(&arg);
		free(arg.line.data);
		arg.line.data = NULL;
		free(arg.line.info);
		arg.line.info = NULL;
		free_ast(arg.ast_head);
		arg.ast_head = NULL;
		terminal_interactive(&term);
	}
	free_arg(&arg);
}
