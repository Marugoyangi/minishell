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

int	exec_redirection(t_node *node,t_env *env, char **path)
{
	if (!ft_strncmp(node->data, ">", 1))
		return (exec_output(node, env, path));
	else if (!ft_strcmp(node->data, ">>"))
		return (exec_append(node, env, path));
	else if (!ft_strcmp(node->data, "<"))
		return (exec_input(node, env, path));
	else
		return (exec_heredoc(node, env, path));
}

int	exec_output(t_node *node, t_env *env, char **path)
{
	int fd;

	// O_TRUNC 이미 파일이 존재하는경우 해당 파일을 비우기때문에 덮어쓰기 효과.
    fd = open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	dup2(fd, 1);
	close(fd);
	if (!exec_command(node, env, path))
		return (0);
	return (1);
}

int	exec_append(t_node *node, t_env *env, char **path)
{
	int fd;

	// O_APPEND 파일이 존재하는 경우 파일 끝에 내용을 추가하기때문에 이어쓰기 효과.
	fd = open(node->argv[0], O_WRONLY | O_CREAT | O_APPEND, 0666);
	dup2(fd, 1);
	close(fd);
	if (!start_exec(node->left, env, path))
		return (0);
	return (1);
}

int	exec_input(t_node *node, t_env *env, char **path)
{
	int fd;

	fd = open(node->argv[0], O_RDONLY);
	dup2(fd, 0);
	close(fd);
	if (!start_exec(node->left, env, path))
		return (0);
	return (1);
}

int	exec_heredoc(t_node *node, t_env *env, char **path)
{
	int fd;

	fd = open(node->filename, O_RDONLY);
	dup2(fd, 0);
	close(fd);
	if (!start_exec(node->left, env, path))
	{
		unlink(node->filename);
		return (0);
	}
	else
	{
		unlink(node->filename);
		return (1);
	}
}
