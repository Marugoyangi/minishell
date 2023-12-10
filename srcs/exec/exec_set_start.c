/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_set_start.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:25:08 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/10 01:25:37 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_exec(t_arg *arg);
void	set_heredoc(t_node *node);
char	**set_path(t_env *env);
int		start_exec(t_node *node, t_env *env, char **path);

void	set_exec(t_arg *arg)
{
	t_node *start;
	t_env	*env;
	char	**path;

	start = arg->ast_head;
	env = arg->envp_head;
	set_heredoc(start);
	path = set_path(env);	// execve 함수를 위한 path 경로 저장.
		
	// 사전준비 완료 후 노드 순회 시작
	start_exec(start, env, path);
}

char	*set_heredoc_filename()
{
	int		fd;
	int		i;
	char	*filename;

	i = 1;
	while(1)
	{
		filename = ft_itoa(i);
		fd = open(filename, O_RDONLY);	// open 성공 = filename 파일 존재
		if (fd == -1)	// 실패시 해당 filename 사용가능;
			return (filename);
		free(filename);
		i++;
	}
}

// heredoc 찾아서 입력값 받은 후 파일에 저장하기
void	set_heredoc(t_node *node)
{
	int		fd;
	char	*line;

	if (!node)
		return ;
	if (node->type == L_REDIRECTION && !ft_strncmp(node->data, "<<", 2))
	{
		node->filename = set_heredoc_filename();
		fd = open(node->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
		while (1)
		{
			line = readline("환경변수 처리");	
			write(fd, line, ft_strlen(line));
		   if (!ft_strcmp(line, node->argv[0]))
			{
				free(line);
				break;
			}
			free(line);
		}
		close(fd);
	}
	set_heredoc(node->left);
	set_heredoc(node->right);
}

// PATH 환경변수 찾아서, 경로배열 스플릿 후 반환.
char	**set_path(t_env *env)
{
	t_env *cur;

	cur = env;
	while (strcmp("PATH", cur->key))
		cur = cur->next;
	return (ft_split(cur->value, ':'));
}

int	start_exec(t_node *node, t_env *env, char **path)
{
	if (!node)
		return (0);
	if (node->type == L_SUBSHELL)
		return (exec_subshell(node, env, path));
	else if (node->type == L_LOGICAL_OPERATOR)
		return (exec_logical_operator(node, env, path));
	else if (node->type == L_PIPELINE)
		return (exec_pipeline(node, env, path));
	else if (node->type == L_REDIRECTION)
		return (exec_redirection(node, env, path));
	else if (node->type == L_SIMPLE_COMMAND)
		return (exec_command(node, env, path));
	else
		return (1);	// 노드가 비어있지않은데 위 분기에 없다면 오류.
}
