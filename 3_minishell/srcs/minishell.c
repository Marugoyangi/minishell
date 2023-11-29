/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:37 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/11/30 07:27:14 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_print_list(t_node *root)
{
	t_node	*tmp;

	tmp = root;
	while (tmp)
	{
		printf("data = %s\n", tmp->data);
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

void	sig_handler(int signum)
{
	(void)signum;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(void)
{
	t_arg			arg;
	struct termios	term;
	struct termios	original_term;

	memset(&arg, 0, sizeof(t_arg)); // ft_memset으로 바꾸자
	tcgetattr(STDOUT_FILENO, &term);
	original_term = term;
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDOUT_FILENO, TCSANOW, &term);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	print_ascii();
	while (1)
	{
		arg.input.data = readline("\033[1;34mminishell$ \033[0m");
		if (!arg.input.data)
		{
			term.c_lflag &= ~(ECHO);
			tcsetattr(STDOUT_FILENO, TCSANOW, &term);
			printf("\033[A\033[11Cexit\n");
			break ;
		}
		add_history(arg.input.data);
		check_line(&arg);
		lexicize(&arg);
		test_print_list(arg.ast_head);
		parser(&arg);
		free(arg.input.data);
	}
	tcsetattr(STDOUT_FILENO, TCSANOW, &original_term);
}
