/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:37 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/10 23:45:04 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_arg 			arg;
	struct termios	term;
	struct termios	original_term;

	if (argc && argv)
		terminal_init(&arg, &term, &original_term, envp);
	print_ascii();
	while (1)
	{
		terminal_interactive(&term);
		arg.line.data = readline(get_ps1(&arg));
		if (!arg.line.data)
		{
			terminal_default(&original_term, 1);
			break ;
		}
		add_history(arg.line.data);
		arg.line.data = modified_strtrim(arg.line.data, " \t\n");
		tokenize(&arg.line);
		lexicize(&arg);
		expand_vars(&arg);
		check_syntax(arg.ast_head, &arg, 0);
		parser(&arg);
		terminal_default(&original_term, 0);
		// set_exec(&arg);
		ft_free((void *)arg.line.data);
		arg.line.data = NULL;
		ft_free((void *)arg.line.info);
		arg.line.info = NULL;
		free_ast(arg.ast_head);
		arg.ast_head = NULL;
	}
	free_arg(&arg);
}
