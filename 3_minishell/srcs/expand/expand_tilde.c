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

void	expand_tilde(int *position, t_line *line, t_arg *arg)
{
	int		i;
	char	*home;

	i = position[0];
	// home = find_envp_node(arg->envp_head, "HOME");
	home = getenv("HOME");
	if (home == NULL)
		home = arg->tilde;
	while (line->info[i])
	{
		while (line->info[i] && line->data[i] != '~')
			i++;
		if (line->info[i] && (line->data[i] == '~' && \
		line->info[i - 1] == T_SPACE) && \
		(line->info[i + 1] == T_SPACE || line->data[i + 1] == '/'))
			i += replace_line(home, line, i, i + 1);
	}
	position[1] = i;
}
