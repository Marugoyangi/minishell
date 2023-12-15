/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:37 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/16 02:55:58 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_fork;

void	set_minishell_path(t_arg *arg, char *path)
{
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;

	tmp1 = ft_strdup(path);
	if (tmp1[0] == '~' || tmp1[0] == '/')
		arg->pwd = tmp1;
	else
	{
		tmp2 = getcwd(NULL, 0);
		if (tmp1[0] == '.' && tmp1[1] == '/')
			tmp3 = modified_strtrim(tmp1, ".");
		else
			tmp3 = ft_strjoin("/", tmp1);
		arg->pwd = modified_strjoin(tmp2, tmp3, 0);
	}
}

void	free_read_line(t_arg *arg)
{
	printf("arg->error->code inside = %d\n", arg->error->code);
	free(arg->line.data);
	arg->line.data = NULL;
	free(arg->line.info);
	arg->line.info = NULL;
	if (arg->error->code == 2)
		free_node(arg->ast_head);
	else
	{
		printf("arg->error->code = %d\n", arg->error->code);
		free_ast(arg->ast_head);
	}
	arg->ast_head = NULL;
	if (arg->error->token)
		free(arg->error->token);
	arg->error->token = NULL;
}

int	lex(t_arg *arg)
{
	tokenize(&arg->line, arg);
	lexicize(arg);
	expand_vars(arg);
	check_syntax(arg->ast_head, arg, 0);
	printf("arg->error->code = %d\n", arg->error->code);
	parser(arg);
	printf("arg->error->code = %d\n", arg->error->code);
	get_heredoc(arg);
	printf("arg->error->code = %d\n", arg->error->code);
	if (arg->error->code == 2)
	{
		printf("argadasd->errordd->code = %d\n", arg->error->code);
		free_read_line(arg);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_arg	arg;

	(void)argc;
	terminal_init(&arg, envp, argv);
	while (1)
	{
		arg.error->code = 0;
		if (!arg.is_subshell)
		{
			arg.line.data = readline(find_env(arg.envp_head, "PS1"));
			if (!arg.line.data)
				terminal_default(1, &arg);
			add_history(arg.line.data);
			arg.line.data = modified_strtrim(arg.line.data, " \t\n");
		}
		if (lex(&arg))
			continue ;
		expand_heredoc(&arg);
		// set_exec(&arg);
		free_read_line(&arg);
		if (arg.is_subshell)
			exit (0);
	}
}
