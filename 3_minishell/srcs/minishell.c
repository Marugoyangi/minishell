/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:37 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/14 08:29:39 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_fork;

int	main(int argc, char **argv, char **envp)
{
	t_arg			arg;
	
	(void )argc;
	terminal_init(&arg, envp);
	print_ascii();
	while (1)
	{
		if (!arg.is_subshell)
		{
			arg.line.data = readline(find_env(arg.envp_head, "PS1"));
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
		check_syntax(arg.ast_head, &arg, 0);
		parser(&arg);
		get_heredoc(&arg);
		if (arg.error->code)
		{
			arg.error->code = 0;
			free(arg.line.data);
			arg.line.data = NULL;
			free(arg.line.info);
			arg.line.info = NULL;
			free_ast(arg.ast_head);
			arg.ast_head = NULL;
			continue ;
		}
		expand_heredoc(&arg);
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
	tcsetattr(STDOUT_FILENO, TCSANOW, &arg.original_term);
}
