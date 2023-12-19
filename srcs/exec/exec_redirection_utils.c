/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:51:44 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/19 23:46:39 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*get_input_node(t_node *node, int *fd)
{
	t_node	*input_node;

	fd[0] = 0;
	input_node = 0;
	while (node)
	{
		if (!ft_strcmp(node->data, "<") || !ft_strcmp(node->data, "<<"))
		{
			input_node = node;
			if (!ft_strcmp(node->data, "<"))
				fd[0] = open(node->argv[0], O_RDONLY);
			else if (!ft_strcmp(node->data, "<<") && node->filename)
				fd[0] = open(node->filename, O_RDONLY);
			if (fd[0] == -1)
				return (input_node);
			close(fd[0]);
		}
		node = node->left;
	}
	return (input_node);
}

t_node	*get_output_node(t_node *node, int *fd)
{
	t_node	*output_node;

	fd[1] = 0;
	output_node = 0;
	while (node)
	{
		if (!ft_strcmp(node->data, ">") || !ft_strcmp(node->data, ">>"))
		{
			output_node = node;
			if (!ft_strcmp(node->data, ">"))
				fd[1] = open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
			else if (!ft_strcmp(node->data, ">>"))
				fd[1] = open(node->argv[0], O_WRONLY | \
				O_CREAT | O_APPEND, 0666);
			if (fd[1] == -1)
				return (output_node);
			close (fd[1]);
		}
		node = node->left;
	}
	return (output_node);
}

int	get_input_fd(t_node *node)
{
	if (!ft_strcmp(node->data, "<"))
		return (open(node->argv[0], O_RDONLY));
	else if (!ft_strcmp(node->data, "<<") && node->filename)
		return (open(node->filename, O_RDONLY));
	return (1);
}

int	get_output_fd(t_node *node)
{
	if (!ft_strcmp(node->data, ">"))
		return (open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666));
	else if (!ft_strcmp(node->data, ">>"))
		return (open(node->argv[0], O_WRONLY | O_CREAT | O_APPEND, 0666));
	return (1);
}

int	error_input_fd(t_node *node)
{
	if (!node)
		return (1);
	if (node->argv && node->argv[0])
		printf("minishell: No such file or directory: %s\n", node->argv[0]);
	return (1);
}
