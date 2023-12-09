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

char *get_ps1(t_arg *arg)
{
	char	*tmp;

	if (find_env(arg->envp_head, "PS1"))
		tmp = find_env(arg->envp_head, "PS1");
	else
		tmp = "minishell";
	arg->ps1 = ft_strjoin("\033[1;34m", ft_strjoin(tmp, "$ "));
	arg->ps1 = ft_strjoin(arg->ps1, "\033[0m");
	return (arg->ps1);
}

void	sig_handler(int signum)
{
	(void)signum;

	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_handler_exec(int signum)
{
	(void)signum;

	printf("\n");
	rl_on_new_line();
	if (signum == SIGQUIT)
		printf("Quit\n");
	rl_replace_line("", 0);
}

void	terminal_default(struct termios *original_term, int exit)
{
	if (exit)
	{
		printf("\033[A\033[11Cexit\n");
		return ;
	}
	signal(SIGQUIT, sig_handler_exec);
	signal(SIGINT, sig_handler_exec);
	tcsetattr(STDOUT_FILENO, TCSANOW, original_term);
}

void	terminal_interactive(struct termios *term)
{
	term->c_lflag &= ~(ECHOCTL);
	tcsetattr(STDOUT_FILENO, TCSANOW, term);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	terminal_init(t_arg *arg, struct termios *term, struct termios *original_term, \
					char **envp)
{
	tcgetattr(STDOUT_FILENO, term);
	*original_term = *term;
	memset(arg, 0, sizeof(t_arg)); // ft_memset으로 바꾸자
	arg->error = (t_error *)malloc(sizeof(t_error));
	arg->error->token = NULL;
	arg->envp_head = init_envp(envp);
	init_shell_vars(arg);
	signal(SIGQUIT, SIG_IGN);
}
