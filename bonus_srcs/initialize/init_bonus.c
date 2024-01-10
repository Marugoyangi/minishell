/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:24:36 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:24:36 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
	if (!find_env(arg->envp_head, "OLDPWD"))
		append_env(arg->envp_head, "OLDPWD", 0);
	if (find_env(arg->envp_head, "HOME"))
		arg->tilde = ft_strdup(find_env(arg->envp_head, "HOME"));
	else if (find_env(arg->envp_head, "USER"))
		arg->tilde = ft_strjoin("/home/", find_env(arg->envp_head, "USER"));
	else
		arg->tilde = ft_strdup(find_env(arg->envp_head, "PWD"));
	arg->last_exit_status = 0;
	init_ps(arg);
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
		if (ps1[i] == '\001' || ps1[i] == '\002')
			len--;
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

void	init_ps(t_arg *arg)
{
	if (tgetnum("colors"))
	{
		arg->ps[0] = ft_strjoin("\001\033[32m\002➜"\
		, "\001\033[36m\002  minishell ");
		arg->ps[0] = modified_strjoin (arg->ps[0], \
		"\001\033[33m\002✗ \001\033[0m\002", 1);
	}
	else
		arg->ps[0] = ft_strdup("➜  minishell ✗ ");
	arg->ps[1] = ft_strdup("> ");
}

void	terminal_init(t_arg *arg, char **envp, char **argv)
{
	if (argv[1] && argv[1][0] != '\n')
	{
		printf("minishell: invalid argument vriables\n");
		exit (1);
	}
	ft_memset(arg, 0, sizeof(t_arg));
	g_recived_signal = 3;
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
	set_minishell_path(arg, argv[0]);
	terminal_interactive(arg);
	if (argv[1] && argv[1][0] == '\n')
		arg->is_subshell = 1;
	else
		print_ascii();
}
