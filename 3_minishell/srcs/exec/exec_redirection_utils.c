/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   exec_redirection_utils.c						   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: woopinbell <woopinbell@student.42.fr>	  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/12/04 19:23:09 by seungwok		  #+#	#+#			 */
/*   Updated: 2023/12/15 13:09:58 by woopinbell	   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

t_node	*get_input_node(t_node *node, int *fd);
t_node	*get_output_node(t_node *node, int *fd);
int		get_input_fd(t_node *node);
int		get_output_fd(t_node *node);
int		error_input_fd(t_node *node);

t_node	*get_input_node(t_node *node, int *fd)
{
	t_node	*input_node;

	fd[0] = 0;
	input_node = 0;
	while (node)
	{
		if (!ft_strcmp(node->data, "<") || !ft_strcmp(node->data, "<<"))
		{
			if (!ft_strcmp(node->data, "<"))
				fd[0] = open(node->argv[0], O_RDONLY);
			else if (!ft_strcmp(node->data, "<<"))
				fd[0] = open(node->filename, O_RDONLY);
			input_node = node;
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

	output_node = 0;
	fd[1] = 0;
	while (node)
	{
		if (!ft_strcmp(node->data, ">") || !ft_strcmp(node->data, ">>"))
		{
			fd[1] = open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
			output_node = node;
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
	else
		return (open(node->filename, O_RDONLY));
}

int	get_output_fd(t_node *node)
{
	if (!ft_strcmp(node->data, ">"))
		return (open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666));
	else
		return (open(node->argv[0], O_WRONLY | O_CREAT | O_APPEND, 0666));
}

int	error_input_fd(t_node *node)
{
	printf("minishell: No such file or directory: %s\n", node->argv[0]);
	return (1);
}
