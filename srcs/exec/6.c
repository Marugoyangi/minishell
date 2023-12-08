/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 19:23:09 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/08 13:10:35 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	exec_output(t_node *node, t_env *env, char **path);
void	exec_append(t_node *node, t_env *env, char **path);
void	exec_input(t_node *node, t_env *env, char **path);
void	exec_heredoc(t_node *node, t_env *env, char **path);

void	exec_redirection(t_node *node,t_env *env, char **path)
{
	if (!strcmp(node->data, ">"))
		exec_output(node, env, path);
	else if (!strcmp(node->data, ">>"))
		exec_append(node, env, path);
	else if (!strcmp(node->data, "<"))
		exec_input(node, env, path);
	else
		exec_heredoc(node, env, path);
}

void	exec_output(t_node *node, t_env *env, char **path)
{
	int fd;
	
	// O_TRUNC 이미 파일이 존재하는경우 해당 파일을 비우기때문에 덮어쓰기 효과.
    fd = open(node->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	dup2(fd, 1);
	close(fd);
	start_exec(node->left, env, path);
}

void	exec_append(t_node *node, t_env *env, char **path)
{
	int fd;

	// O_APPEND 파일이 존재하는 경우 파일 끝에 내용을 추가하기때문에 이어쓰기 효과.
	fd = open(node->argv[0], O_WRONLY | O_CREAT | O_APPEND, 0666);
	dup2(fd, 1);
	close(fd);
	start_exec(node->left, env, path);	
}

void	exec_input(t_node *node, t_env *env, char **path)
{
	int fd;

	fd = open(node->argv[0], O_RDONLY);
	dup2(fd, 0);
	close(fd);
	start_exec(node->left, env, path);
}

void	exec_heredoc(t_node *node, t_env *env, char **path)
{
	int fd;

	fd = open(node->argv[0], O_RDONLY);
	dup2(fd, 0);
	close(fd);
	start_exec(node->left, env, path);
}