/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 06:05:31 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/08 06:05:31 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int filterUTF8(const char* str)
{
	unsigned char byte;
	int		unicode_len;
	int		ascii_len;
	
	unicode_len = 0;
	ascii_len = 0;
	while (*str)
	{
		byte = (unsigned char)(*str);
		if ((*str & 0xC0) != 0x80)
		 {
			if ((byte & 0xE0) == 0xC0)
			{
				unicode_len += 2;
				ascii_len += 1;
			}
			else if ((byte & 0xF0) == 0xE0)
			{
				unicode_len += 3;
				ascii_len += 1;
			}
			else if ((byte & 0xF8) == 0xF0)
			{
				unicode_len += 4;
				ascii_len += 1;
			}
		}
		str++;
	}
	return (unicode_len - ascii_len);
}

int	ps_len(char *ps1)
{
	int	i;
	int	colored;
	int	len;

	i = 0;
	colored = 0;
	len = 0;
	while (ps1[i])
	{
		if (ps1[i] == '\033')
			colored = 1;
		else if (colored == 1 && (ps1[i] == 'm' || \
		ps1[i] == 'H' || ps1[i] == 'J' || ps1[i] == 'K' || \
		ps1[i] == 'A' || ps1[i] == 'B' || ps1[i] == 'D' || \
		ps1[i] == 'C'))
			colored = 0;
		else if (colored == 0)
			len++;
		i++;
	}
	return (len);
}


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

	if (signum == SIGQUIT)
		rl_replace_line("", 0);
	else if (signum == SIGINT)
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

	if (g_signal_fork == 1)
	{
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, SIG_IGN);
	}
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

void	terminal_default(int exit, t_arg *arg)
{
	int		ps1_len;

	if (g_signal_fork == 1)
		g_signal_fork = 0;
	else if (g_signal_fork == 0)
	{
		tcsetattr(STDOUT_FILENO, TCSANOW, &arg->original_term);
		signal(SIGINT, sig_handler_exec);
		signal(SIGQUIT, sig_handler_exec);
	}
	else if (exit)
	{
		ps1_len = ps_len(find_env(arg->envp_head, "PS1"));
		ps1_len = ps1_len - filterUTF8(find_env(arg->envp_head, "PS1"));
		printf("\033[A\033[%dC exit\n", ps1_len);
		return ;
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

void	terminal_init(t_arg *arg, char **envp)
{
	memset(arg, 0, sizeof(t_arg)); // ft_memset으로 바꾸자
	tcgetattr(STDOUT_FILENO, &arg->term);
	arg->original_term = arg->term;
	arg->term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDOUT_FILENO, TCSANOW, &arg->term);
	arg->error = (t_error *)malloc(sizeof(t_error));
	if (!arg->error)
		exit(1);
	arg->error->token = NULL;
	arg->envp_head = init_envp(envp);
	init_shell_vars(arg);
	terminal_interactive(arg);
}
