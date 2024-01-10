/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:22:11 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:22:11 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	waitpid_signal(t_arg *arg, int *status)
{
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		*status = WTERMSIG(*status) + 128;
	terminal_interactive(arg);
}

int	ft_delete_line_free(t_line **line)
{
	if ((*line)->data)
		free((*line)->data);
	(*line)->data = NULL;
	if ((*line)->info)
		free((*line)->info);
	(*line)->info = NULL;
	free(*line);
	*line = NULL;
	return (0);
}

void	heredoc_variables_init(int *status, pid_t *pid, int *xy)
{
	*status = 0;
	*pid = 0;
	xy[1] = -1;
	xy[0] = 1;
}
