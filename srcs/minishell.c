/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:37 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/10 01:17:03 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_print_list(t_node *root)
{
	t_node	*tmp;

	tmp = root;
	while (tmp)
	{
		printf("argv ----------------\n");
		printf("data: %s\n", tmp->data);
		if (tmp->type == L_SUBSHELL)
			printf("type = L_SUBSHELL\n");
		else if (tmp->type == L_REDIRECTION)
			printf("type = L_REDIRECTION\n");
		else if (tmp->type == L_SIMPLE_COMMAND)
			printf("type = L_SIMPLE_COMMAND\n");
		else if (tmp->type == L_PIPELINE)
			printf("type = L_PIPELINE\n");
		else if (tmp->type == L_LOGICAL_OPERATOR)
			printf("type = L_LOGICAL_OPERATOR\n");
		tmp = tmp->right;
	}
}

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
		arg.line.data = ft_strtrim(arg.line.data, " \t");
		tokenize(&arg.line);
		lexicize(&arg);
		test_print_list(arg.ast_head);
		expand_vars(&arg);
		check_syntax(arg.ast_head, &arg, 0);
		parser(&arg);
		set_exec(&arg);
		free(arg.line.data);
	}
	terminal_default(&original_term, 0);
}
