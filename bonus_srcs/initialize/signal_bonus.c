/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 07:12:27 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:24:13 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	signal_ignore(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_handler(int signum)
{
	(void)signum;
	if (signum == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_recived_signal = signum;
	}
	else if (signum == SIGQUIT)
		g_recived_signal = signum;
}

void	sig_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		g_recived_signal = signum;
	}
}

void	sig_handler_waiting(int signum)
{
	(void)signum;
	if (g_recived_signal == SIGINT)
		exit (130);
	else if (g_recived_signal == SIGQUIT)
		exit (131);
}

void	sig_handler_exec(int signum)
{
	if (signum == SIGQUIT)
	{
		rl_on_new_line();
		printf("Quit: 3\n");
		rl_replace_line("", 0);
		g_recived_signal = signum;
	}
	else if (signum == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		g_recived_signal = signum;
	}
}
