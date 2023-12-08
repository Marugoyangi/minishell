/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 05:51:56 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/05 16:54:15 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_number(char c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}

int	is_delimiter(char c)
{
	if (('a' < c && c < 'z') || c == '_' || ('A' < c && c < 'Z'))
		return (1);
	return (0);
}

void	expand_envp(int *position, t_line *line, t_arg *arg)
{
	int		i;
	int		start;
	char	*env;

	start = -1;
	i = position[0];
	while (i < position[1] && line->info[i])
	{
		while (line->info[i] && line->info[i] != T_ENV)
			i++;
		while (line->info[i] && (line->info[i] == T_ENV))
			i++;
		if (i > 0 && line->info[i - 1] == T_ENV && line->info[i])
			start = i - 1;
		if (i > 0 && line->info[i - 1] == T_ENV && !is_number(line->data[i]))
			i++;
		else
			start = - 1;
		while (line->info[i] && (is_delimiter(line->data[i]) || is_number(line->data[i])))
			i++;
		env = NULL;
		if (start != -1)
		{
			env = ft_substr(line->data, start + 1, i - start - 1);
			if (ft_strcmp(env, "?") == 0)
				env = ft_itoa(arg->last_exit_status);
			else
				env = find_env(arg->envp_head, env);
			if (!env)
			{
				ft_delete_line(i - start, line, i);
				position[1] -= i - start;
				i -= i - start;
			}
			else
			{
				position[1] += ft_strlen(env) - (i - start);
				i += replace_line(env, line, start, i) - (i - start) + 1;
			}
			start = -1;
		}
	}
}

void	printfline(t_line *line)
{
	int	i;

	i = 0;
	while (line->info[i])
	{
		printf("[%d]", line->info[i]);
		i++;
	}
	printf("\n");
}

void	expand_vars(int *position, t_line *line, t_arg *arg)
{
	expand_envp(position, line, arg);
	expand_tilde(position, line, arg);
	remove_quotes(position, line);
	expand_asterisk(position, line);
}
