/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 22:08:36 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/02 22:08:36 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quotes(t_line *line)
{
	int		i;
	int		start;

	i = 0;
	while (line && line->data && line->info && line->info[i])
	{
		while (line->info[i] && line->info[i] != T_SINGLE_QUOTE && \
		line->info[i] != T_DOUBLE_QUOTE)
			i++;
		start = i;
		while (line->info && line->info[i] && (line->info[i] == T_SINGLE_QUOTE || \
			line->info[i] == T_DOUBLE_QUOTE))
			i++;
		ft_delete_line(i - start, line, i);
		i -= i - start - 1;
	}
}

void	expand_tilde(t_line *line, t_arg *arg)
{
	int		i;
	char	*home;
	t_line	*tilde_line;

	i = -1;
	home = find_env(arg->envp_head, "HOME");
	if (!home)
		home = arg->tilde;
	while (line && line->info && line->info[++i])
	{
		if (line->info[i] && (line->info[i] == T_LITERAL || line->data[i] != '~'))
			i++;
		else if ((line->data[i] == '~' && line->info[i] == T_WORD) && \
		((i > 0 && (line->info[i - 1] == T_SPACE)) || i == 0) && \
		((line->info[i + 1] == T_SPACE || line->info[i + 1] == '\0' || \
		line->data[i + 1] == '/')))
		{
			tilde_line = get_line_info(home);
			i += replace_line(tilde_line, line, i, i + 1) - 1;
		}
	}
}
