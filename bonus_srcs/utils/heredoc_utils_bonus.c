/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 04:38:34 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/20 04:38:34 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	heredoc_variables_init(int *status, pid_t *pid, int *xy)
{
	*status = 0;
	*pid = 0;
	xy[1] = -1;
	xy[0] = 1;
}
