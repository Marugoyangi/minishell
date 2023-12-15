/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 07:12:27 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/16 03:41:46 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int signum)
{
	(void)signum;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sig_handler_heredoc(int signum)
{
	(void)signum;
	if (signum == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		exit (1);
	}
}

void	sig_handler_exec(int signum)
{
	(void)signum;
	if (signum == SIGQUIT)
	{
		rl_on_new_line();
		printf("Quit: 3\n");
		rl_replace_line("", 0);
		if (g_signal_fork == 1)
			exit (131);
	}
	else if (signum == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		if (g_signal_fork == 1)
			exit (130);
	}
}

void	terminal_default(int exit_flag, t_arg *arg)
{
	int		ps1_len;

	if (exit_flag)
	{
		ps1_len = ps_len(find_env(arg->envp_head, "PS1"));
		ps1_len = ps1_len - filter_utf8(find_env(arg->envp_head, "PS1"));
		printf("\033[A\033[%dC exit\n", ps1_len);
		free_arg(arg);
		tcsetattr(STDOUT_FILENO, TCSANOW, &arg->original_term);
		exit (0);
	}
	if (g_signal_fork == 1)
	{
		tcsetattr(STDOUT_FILENO, TCSANOW, &arg->original_term);
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}

	else if (g_signal_fork == 0)
	{
		tcsetattr(STDOUT_FILENO, TCSANOW, &arg->original_term);
		signal(SIGINT, sig_handler_exec);
		signal(SIGQUIT, sig_handler_exec);
	}
}

void	terminal_interactive(t_arg *arg)
{
	if (g_signal_fork == 1)
	{
		signal(SIGINT, sig_handler_exec);
		signal(SIGQUIT, sig_handler_exec);
	}
	else
	{
		tcsetattr(STDOUT_FILENO, TCSANOW, &arg->term);
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, SIG_IGN);
	}
}
