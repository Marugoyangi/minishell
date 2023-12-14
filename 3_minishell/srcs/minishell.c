/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:37 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/15 00:00:41 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_fork;

int	main(int argc, char **argv, char **envp)
{
	t_arg			arg;
	char			*prompt;
	
	(void )argc;
	terminal_init(&arg, envp);
	print_ascii();
	while (1)
	{
		if (!arg.is_subshell)
		{
			prompt = find_env(arg.envp_head, "PS1");
			if (!prompt)
				prompt = ">";
			arg.line.data = readline(prompt);
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
