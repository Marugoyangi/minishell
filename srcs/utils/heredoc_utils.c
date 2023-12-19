/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 17:08:50 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/19 17:08:50 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_variables_init(int *status, pid_t *pid, int *xy)
{
	*status = 0;
	*pid = 0;
	xy[1] = -1;
	xy[0] = 1;
}
