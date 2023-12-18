/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 05:27:50 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/17 05:27:50 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	terminal_default(t_arg *arg)
{
	int		ps1_len;

	ps1_len = ps_len(find_env(arg->envp_head, "PS1"));
	ps1_len = ps1_len - filter_utf8(find_env(arg->envp_head, "PS1"));
	printf("\033[A\033[%dC exit\n", ps1_len);
	free_arg(arg);
	tcsetattr(STDOUT_FILENO, TCSANOW, &arg->original_term);
	exit (0);
}

void	terminal_interactive(t_arg *arg)
{
	tcsetattr(STDOUT_FILENO, TCSANOW, &arg->term);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
}
