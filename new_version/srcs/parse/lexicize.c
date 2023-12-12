/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexicize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 03:10:34 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/12 19:54:46 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*redirection(int *i, t_line *line)
{
	int		position[2];
	char	*redirection;

	position[0] = *i;
	if (line->data[*i] == '>' && line->data[*i + 1] == '>' && line->info[*i + 1] == T_OPERATOR)
		redirection = ft_strdup(">>");
	else if (line->data[*i] == '<' && line->data[*i + 1] == '<' && line->info[*i + 1] == T_OPERATOR)
		redirection = ft_strdup("<<");
	else if (line->data[*i] == '>')
		redirection = ft_strdup(">");
	else if (line->data[*i] == '<')
		redirection = ft_strdup("<");
	*i = *i + ft_strlen(redirection);
	free(redirection);
	while (line->info[*i] == T_SPACE)
		(*i)++;
	while (line->info[*i] == T_DOUBLE_QUOTE || line->info[*i] == T_SINGLE_QUOTE \
	|| line->info[*i] == T_WORD || line->info[*i] == T_LITERAL || \
	line->info[*i] == T_ENV || line->info[*i] == T_ASTERISK)
		(*i)++;
	position[1] = *i;
	return (create_node(ft_substr(line->data, position[0], \
	position[1] - position[0]), subline(line, position[0], position[1]) \
	, L_REDIRECTION));
}

t_node	*subshell(int *i, t_line *line)
{
	int	position[2];

	position[0] = *i;
	*i = *i + 1;
	while (line->info[*i] != T_PARENTHESIS)
		(*i)++;
	*i = *i + 1;
	position[1] = *i;
	return (create_node(ft_substr(line->data, position[0], \
	position[1] - position[0]), subline(line, position[0], position[1]) \
	, L_SUBSHELL));
}

t_node	*simple_cmd(int *i, t_line *line)
{
	int		position[2];

	position[0] = *i;
	while (line->info[*i] == T_WORD || line->info[*i] == T_LITERAL || \
	line->info[*i] == T_DOUBLE_QUOTE || line->info[*i] == T_SINGLE_QUOTE || \
	line->info[*i] == T_ENV || \
	line->info[*i] == T_ASTERISK)
		(*i)++;
	position[1] = *i;
	return (create_node(ft_substr(line->data, position[0], \
	position[1] - position[0]), subline(line, position[0], position[1]) \
	, L_SIMPLE_COMMAND));
}

t_node	*check_type(t_line *line, int *i)	// 타입 확인 후 노드 생성
{
	if (line->info[*i] == T_OPERATOR && \
	(line->data[*i] == '&' && line->data[*i + 1] == '&'))
	{
		*i = *i + 2;
		return (create_node("&&", NULL, L_LOGICAL_OPERATOR));	// 논리연산자 노드 생성
	}
	else if (line->info[*i] == T_OPERATOR && \
	(line->data[*i] == '|' && line->data[*i + 1] == '|'))
	{
		*i = *i + 2;
		return (create_node("||", NULL, L_LOGICAL_OPERATOR));	// 논리연산자 노드 생성
	}
	else if (line->info[*i] == T_OPERATOR && \
	(line->data[*i] == '|' && line->data[*i + 1] != '|'))
	{
		*i = *i + 1;
		return (create_node("|", NULL, L_PIPELINE));	// 파이프 노드 생성
	}
	else if (line->info[*i] == T_WORD || line->info[*i] == T_LITERAL \
	|| line->info[*i] == T_DOUBLE_QUOTE || line->info[*i] == T_SINGLE_QUOTE \
	|| line->info[*i] == T_ENV || line->info[*i] == T_ASTERISK)
		return (simple_cmd(i, line));	// 단순 명령어 노드 생성
	else if (line->info[*i] == T_PARENTHESIS)
		return (subshell(i, line));	// 서브셸 노드 생성
	else if (line->info[*i] == T_OPERATOR && 
	(line->data[*i] == '>' || line->data[*i] == '<'))
		return (redirection(i, line));	// 리다이렉션 노드 생성
	return (NULL);
}

void	lexicize(t_arg *arg)	// 왼쪽부터 순서대로 이어진 연결리스트 만들기
{
	t_node	*tmp;
	int		i;

	i = 0;
	if (arg->line.data[0] == '\0')
		return ;
	arg->ast_head = check_type(&arg->line, &i);	// 첫번째 노드 생성
	tmp = arg->ast_head;
	while (tmp && arg->line.info[i] != '\0')
	{
		while (arg->line.info[i] == T_SPACE)
			i++;
		tmp->right = check_type(&arg->line, &i);	// 두번째 노드 생성....
		if (tmp->right)
		{
			tmp->right->left = tmp;
			tmp = tmp->right;
		}
	}
}
