/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:24:20 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:24:20 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

char	*get_ps(t_arg *arg, int no)
{
	char	*tmp;

	if (no == 1)
	{
		tmp = find_env(arg->envp_head, "PS1");
		if (!tmp)
			return (arg->ps[0]);
		else
			return (tmp);
	}
	else
	{
		tmp = find_env(arg->envp_head, "PS2");
		if (!tmp)
			return (arg->ps[1]);
		else
			return (tmp);
	}
	return (NULL);
}

void	terminal_default(t_arg *arg)
{
	int		ps1_len;

	ps1_len = ps_len(get_ps(arg, 1));
	ps1_len = ps1_len - filter_utf8(get_ps(arg, 1));
	printf("\001\033[A\033[%dC\002exit\n", ps1_len);
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
