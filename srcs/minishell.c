/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:37 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/17 22:30:36 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_recived_signal;

void	set_minishell_path(t_arg *arg, char *path)
{
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;

	tmp1 = ft_strdup(path);
	if (tmp1[0] == '~' || tmp1[0] == '/')
		arg->minishell_path = tmp1;
	else
	{
		tmp2 = getcwd(NULL, 0);
		if (tmp1[0] == '.' && tmp1[1] == '/')
			tmp3 = modified_strtrim(tmp1, ".");
		else
			tmp3 = ft_strjoin("/", tmp1);
		arg->minishell_path = modified_strjoin(tmp2, tmp3, 0);
	}
}

void	free_read_line(t_arg *arg)
{
	free(arg->line.data);
	arg->line.data = NULL;
	free(arg->line.info);
	arg->line.info = NULL;
	if (arg->error->code == 2 && arg->ast_head)
		free_node(arg->ast_head);
	else
		free_ast(arg->ast_head);
	arg->ast_head = NULL;
	if (arg->error->token)
		free(arg->error->token);
	arg->error->token = NULL;
	arg->last_exit_status = arg->error->code;
	arg->error->code = 0;
}

int	lex(t_arg *arg)
{
	tokenize(&arg->line, arg);
	if (arg->error->code)
	{
		free_read_line(arg);
		if (!arg->is_subshell)
			return (1);
		else if (arg->is_subshell)
			exit (1);
	}
	lexicize(arg);
	expand_vars(arg);
	if (!arg->is_subshell)
		check_syntax(arg->ast_head, arg, 0);
	parser(arg);
	get_heredoc(arg);
	if (arg->error->code)
	{
		free_read_line(arg);
		if (!arg->is_subshell)
			return (1);
		else if (arg->is_subshell)
			exit (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_arg	arg;

	terminal_init(&arg, envp, argv);
	while (1)
	{
		if (!arg.is_subshell && argc)
		{
			rl_replace_line("", 0);
			arg.line.data = readline(find_env(arg.envp_head, "PS1"));
			if (!arg.line.data)
				terminal_default(&arg);
			add_history(arg.line.data);
			arg.line.data = modified_strtrim(arg.line.data, " \t\n");
		}
		else
			arg.line.data = ft_strdup(argv[2]);
		if (lex(&arg))
			continue ;
		expand_heredoc(&arg);
		set_exec(&arg);
		free_read_line(&arg);
		if (arg.is_subshell)
			exit (0);
	}
}
