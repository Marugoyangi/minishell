/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 19:23:09 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/10 01:29:00 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int exec_redirection(t_node *node, t_env *env, char **path);
int exec_output(t_node *node, t_env *env, char **path);
int exec_append(t_node *node, t_env *env, char **path);
int exec_input(t_node *node, t_env *env, char **path);
int exec_heredoc(t_node *node, t_env *env, char **path);

// 수정할 사항
// 내장함수 리다이렉션시 무한루프
// 외장함수 리다이렉션시 fd 서로 공유 안되는듯함.

int	exec_redirection(t_node *node,t_env *env, char **path)
{
	printf("here\n");
	if (!ft_strncmp(node->data, ">", 1))
		return (exec_output(node, env, path));
	else if (!ft_strncmp(node->data, ">>", 2))
		return (exec_append(node, env, path));
	else if (!ft_strncmp(node->data, "<", 1))
		return (exec_input(node, env, path));
	else if (!ft_strncmp(node->data, "<<", 2))
		return (exec_heredoc(node, env, path));
	return (1);
}

int	exec_output(t_node *node, t_env *env, char **path)
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
		return (check_built_in(node, env));
	}
	return (external_command_redirection(node, path, fd, 1));
}

int	exec_append(t_node *node, t_env *env, char **path)
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
		return (check_built_in(node, env));
	}
	return (external_command_redirection(node, path, fd, 1));
}

int	exec_input(t_node *node, t_env *env, char **path)
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
		return (check_built_in(node, env));
	}
	return (external_command_redirection(node, path, fd, 0));
}

int	exec_heredoc(t_node *node, t_env *env, char **path)
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
		return_value = check_built_in(cur, env);
		unlink(node->filename);
		return (return_value);
	}
	return_value = external_command_redirection(cur, path, fd, 0);
	unlink(node->filename);
	return (return_value);
}
