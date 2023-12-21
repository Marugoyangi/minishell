/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 15:47:48 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/20 01:42:54 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*set_heredoc_filename(int *i)
{
	int		fd;
	char	*filename;

	while (1)
	{
		filename = modified_strjoin(".", ft_itoa(*i), 2);
		fd = open(filename, O_RDONLY);
		if (fd == -1)
		{
			(*i)++;
			return (filename);
		}
		close(fd);
		free(filename);
		(*i)++;
	}
}

void	get_heredoc_filename(t_node *root, int *i, t_arg *arg)
{
	t_node	*node;

	if (!root)
		return ;
	node = root;
	if (node->type == L_REDIRECTION)
	{
		if (node->data && !ft_strcmp(node->data, "<<") && \
		node->argv && node->argv[0])
			node->filename = set_heredoc_filename(i);
	}
	get_heredoc_filename(node->right, i, arg);
}
