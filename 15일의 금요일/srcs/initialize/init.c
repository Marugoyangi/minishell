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

void	init_shell_vars(t_arg *arg)
{
	char	*tmp;

	if (find_env(arg->envp_head, "SHLVL"))
	{
		tmp = ft_itoa(ft_atoi(find_env(arg->envp_head, "SHLVL")) + 1);
		set_env(arg->envp_head, "SHLVL", tmp);
		free(tmp);
	}
	else
		append_env(arg->envp_head, "SHLVL", "1");
	if (find_env(arg->envp_head, "HOME"))
		arg->tilde = ft_strdup(find_env(arg->envp_head, "HOME"));
	else if (find_env(arg->envp_head, "USER"))
		arg->tilde = ft_strjoin("/home/", find_env(arg->envp_head, "USER"));
	else
		arg->tilde = getcwd(NULL, 0);
	arg->last_exit_status = 0;
}

int	filter_utf8(const char	*str)
{
	unsigned char	byte;
	int				unicode_len;

	unicode_len = 0;
	while (*str)
	{
		byte = (unsigned char)(*str);
		if ((*str & 0xC0) != 0x80 && (byte & 0xE0) == 0xC0)
			unicode_len += 1;
		else if ((*str & 0xC0) != 0x80 && (byte & 0xF0) == 0xE0)
			unicode_len += 2;
		else if ((*str & 0xC0) != 0x80 && (byte & 0xF8) == 0xF0)
			unicode_len += 3;
		str++;
	}
	return (unicode_len);
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

void	terminal_init(t_arg *arg, char **envp, char **argv)
{
	memset(arg, 0, sizeof(t_arg)); // memset
	tcgetattr(STDOUT_FILENO, &arg->term);
	arg->original_term = arg->term;
	arg->term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDOUT_FILENO, TCSANOW, &arg->term);
	arg->error = (t_error *)ft_malloc(sizeof(t_error));
	arg->error->token = NULL;
	arg->error->code = 0;
	arg->error->type = 0;
	arg->envp_head = init_envp(envp);
	init_shell_vars(arg);
	terminal_interactive(arg);
	arg->pwd = ft_strdup(argv[0]);
	if (argv[1] && argv[1][0] == '\n')
	{
		arg->is_subshell = 1;
		arg->pwd = argv[0];
	}
	else
		print_ascii();
}
