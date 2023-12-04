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

void	remove_quotes(int *position, t_line *line)
{
	int		i;
	int		start;

	i = position[0];
	while (i < position[1] && line->info[i])
	{
		while (line->info[i] && line->info[i] != T_SINGLE_QUOTE && \
		line->info[i] != T_DOUBLE_QUOTE)
			i++;
		start = i;
		while (line->info[i] && i < position[1] && \
		(line->info[i] == T_SINGLE_QUOTE || \
		line->info[i] == T_DOUBLE_QUOTE))
			i++;
		ft_delete_line(i - start, line, start);
		i -= i - start;
		position[1] -= i - start;
	}
}

void	expand_tilde(int *position, t_line *line, t_arg *arg)
{
	int		i;
	char	*home;

	i = position[0];
	if (arg->tilde)
		home = arg->tilde;
	else
		home = find_env(arg->envp_head, "HOME");
	if (home == NULL)
		home = getcwd(NULL, 0);
	while (i < position[1] && line->info[i])
	{
		if (line->info[i] && (line->info[i] == T_LITERAL || line->data[i] != '~'))
			i++;
		else if ((line->data[i] == '~' && line->info[i] == T_WORD && \
		(line->info[i - 1] == T_SPACE || i == 0)) && \
		(line->info[i + 1] == T_SPACE || line->info[i + 1] == '\0' || \
		line->data[i + 1] == '/'))
		{
			i += replace_line(home, line, i, i + 1);
			position[1] += ft_strlen(home);
		}
		else
			i++;
	}
	printf("line->data : %s\n", line->data);
}
