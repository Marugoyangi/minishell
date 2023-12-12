/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungwok <seungwok@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 19:23:09 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/12 13:03:52 by seungwok         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int exec_redirection(t_node *node, t_arg *arg);
int exec_output(t_node *node, t_arg *arg);
int exec_append(t_node *node, t_arg *arg);
int exec_input(t_node *node, t_arg *arg);
int exec_heredoc(t_node *node, t_arg *arg);

int	exec_redirection(t_node *node, t_arg *arg)
{
	if (!ft_strcmp(node->data, ">"))
		return (exec_output(node, arg));
	else if (!ft_strcmp(node->data, ">>"))
		return (exec_append(node, arg));
	else if (!ft_strcmp(node->data, "<"))
		return (exec_input(node, arg));
	else if (!ft_strcmp(node->data, "<<"))
		return (exec_heredoc(node, arg));
	return (1);
}

int	exec_output(t_node *node, t_arg *arg)
{
	int		fd;

	fd = open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	node = node->right;
	if (!node->data) // 하위 명령어가 없어도 노드가 생성되는 오류로 인하여 node대신 data참조
		return (0);
	if (!check_built_in_redirection(node))
	{
		dup2(fd, 1);
		close(fd);
		return (check_built_in(node, arg));
	}
	return (external_command_redirection(node, arg, fd, 1));
}

int	exec_append(t_node *node, t_arg *arg)
{
	int		fd;

	fd = open(node->argv[0], O_WRONLY | O_CREAT | O_APPEND, 0666);
	node = node->right;
	if (!node->data)
		return (0);
	if (!check_built_in_redirection(node))
	{
		dup2(fd, 1);
		close(fd);
		return (check_built_in(node, arg));
	}
	return (external_command_redirection(node, arg, fd, 1));
}

int	exec_input(t_node *node, t_arg *arg)
{
	int fd;

	fd = open(node->argv[0], O_RDONLY);
	node = node->right;
	if (!node->data)
		return (0);
	if (!check_built_in_redirection(node))
	{
		dup2(fd, 0);
		close(fd);
		return (check_built_in(node, arg));
	}
	return (external_command_redirection(node, arg, fd, 0));
}

int	exec_heredoc(t_node *node, t_arg *arg)
{
	int		fd;
	int		return_value;
	t_node	*cur;

	fd = open(node->filename, O_RDONLY);
	cur = node->right;
	if (!cur->data)
		return (0);
	if (!check_built_in_redirection(node))
	{
		dup2(fd, 0);
		close(fd);
		return_value = check_built_in(cur, arg);
		unlink(node->filename);
		return (return_value);
	}
	return_value = external_command_redirection(cur, arg, fd, 0);
	unlink(node->filename);
	return (return_value);
}
