/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 15:08:03 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/08 11:25:24 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    set_heredoc(t_node *node);
char	**set_path(t_env *env);
void    start_exec(t_node *node, t_env *env, char **path);

void    set_exec(t_arg *arg)
{
    t_node *start;
    t_env	*env;
    char    **path;

    start = arg->ast_head;
    env = arg->envp_head;

    set_heredoc(start);
    path = set_path(env);
    
    // 사전준비 완료 후 노트 순회 시작
    start_exec(start, env, path);
}

// heredoc 찾아서 입력값 받은 후 파일에 저장하기
void    set_heredoc(t_node *node)
{
    int     fd;
    char    *line;

    if (!node)
        return ;
    if (node->type == L_REDIRECTION && !ft_strncmp(node->data, "<<", 2))
    {
        fd = open(node->argv[0], O_WRONLY | O_CREAT , 0666);
        while (1)
        {
            line = readline("\033[1;34mheredoc> \033[0m");
            write(fd, line, ft_strlen(line));
           if (ft_strncmp(line, node->argv[0], ft_strlen2(node->argv[0])))
            {
                free(line);
                break;
            }
            free(line);
        }
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

void    start_exec(t_node *node, t_env *env, char **path)
{
    if (!node)
        return ;
    
    if (node->type == L_SUBSHELL)
        exec_subshell(node, env, path);
    else if (node->type == L_REDIRECTION)
        exec_redirection(node, env, path);
    else if (node->type == L_SIMPLE_COMMAND)
        exec_command(node, env, path);
    else if (node->type == L_PIPELINE)
        exec_pipeline(node, env, path);
    else if (node->type == L_LOGICAL_OPERATOR)
        exec_logical_operator(node, env, path);
}
