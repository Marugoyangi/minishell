/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:37 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/13 16:07:49 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_fork;

int	main(int argc, char **argv, char **envp)
{
	t_arg 			arg;
	
	(void )argc;
	terminal_init(&arg, envp);
	terminal_interactive(&arg);
	print_ascii();
	while (1)
	{
		if (!arg.is_subshell)
		{
			arg.line.data = readline(get_ps1(&arg));
			if (!arg.line.data)
			{
				terminal_default(1, &arg);
				break ;
			}
			add_history(arg.line.data);
			arg.line.data = modified_strtrim(arg.line.data, " \t\n");
		}
		else
		{
			arg.line.data = ft_strdup(argv[0]);
			arg.line.data = modified_strtrim(arg.line.data, "()");
		}
		tokenize(&arg.line, &arg);
		lexicize(&arg);
		expand_vars(&arg);
		if (check_syntax(arg.ast_head, &arg, 0))
			continue ; 
		parser(&arg);
		set_exec(&arg);
		free(arg.line.data);
		arg.line.data = NULL;
		free(arg.line.info);
		arg.line.info = NULL;
		free_ast(arg.ast_head);
		arg.ast_head = NULL;
		if (arg.is_subshell)
			exit (0);
	}
	free_arg(&arg);
}
