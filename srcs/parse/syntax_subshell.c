/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 21:23:41 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/06 21:23:41 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_subshell(t_node *node, t_arg *arg)
{
	t_arg tmp;

	memset(&tmp, 0, sizeof(t_arg));
	tmp.line.data = ft_strdup(node->data);
	tmp.line.data = ft_strtrim(tmp.line.data, "()");
	check_line(&tmp);
	lexicize(&tmp, 1);
	if (check_syntax(tmp.ast_head, arg, 1))
		return (1);
	return (0);
}
