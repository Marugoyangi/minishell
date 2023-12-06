/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexicize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 03:10:34 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/07 05:38:41 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*redirection(int *i, t_line *line, t_arg *arg, int flag)
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
	while (line->info[*i] == T_SPACE)
		(*i)++;
	while (line->info[*i] == T_DOUBLE_QUOTE || line->info[*i] == T_SINGLE_QUOTE \
	|| line->info[*i] == T_WORD || line->info[*i] == T_LITERAL || \
	line->info[*i] == T_ENV || line->info[*i] == T_ASTERISK)
		(*i)++;
	position[1] = *i;
	*i = position[0];
	if (flag == 0)
		expand_vars(position, line, arg);
	*i += ft_strlen(ft_substr(line->data, position[0], \
	position[1] - position[0]));
	return (create_ast_node(line, position[0], position[1], L_REDIRECTION));
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
	*i = position[0];
	return (create_node(ft_substr(line->data, position[0], \
	position[1] - position[0]), L_SUBSHELL, i));
}

t_node	*simple_cmd(int *i, t_line *line, t_arg *arg, int flag)
{
	int		position[2];

	if (arg)
	position[0] = *i;
	while (line->info[*i] == T_WORD || line->info[*i] == T_LITERAL || \
	line->info[*i] == T_SPACE || line->info[*i] == T_DOUBLE_QUOTE || \
	line->info[*i] == T_SINGLE_QUOTE || line->info[*i] == T_ENV || \
	line->info[*i] == T_ASTERISK)
		(*i)++;
	position[1] = *i;
	*i = position[0];
	if (flag == 0)
		expand_vars(position, line, arg);
	*i += ft_strlen(ft_substr(line->data, position[0], \
	position[1] - position[0]));
	return (create_ast_node(line, position[0], position[1], L_SIMPLE_COMMAND));
}

t_node	*check_type(t_line *line, int *i, t_arg *arg, int flag)	// 타입 확인 후 노드 생성
{
	if (line->info[*i] == T_OPERATOR && \
	(line->data[*i] == '&' && line->data[*i + 1] == '&'))
		return (create_node("&&", L_LOGICAL_OPERATOR, i));	// 논리연산자 노드 생성
	else if (line->info[*i] == T_OPERATOR && \
	(line->data[*i] == '|' && line->data[*i + 1] == '|'))
		return (create_node("||", L_LOGICAL_OPERATOR, i));	// 논리연산자 노드 생성
	else if (line->info[*i] == T_OPERATOR && \
	(line->data[*i] == '|' && line->data[*i + 1] != '|'))
		return (create_node("|", L_PIPELINE, i));	// 파이프 노드 생성
	else if (line->info[*i] == T_WORD || line->info[*i] == T_LITERAL \
	|| line->info[*i] == T_DOUBLE_QUOTE || line->info[*i] == T_SINGLE_QUOTE \
	|| line->info[*i] == T_ENV || line->info[*i] == T_ASTERISK)
		return (simple_cmd(i, line, arg, flag));	// 단순 명령어 노드 생성
	else if (line->info[*i] == T_PARENTHESIS)
		return (subshell(i, line));	// 서브셸 노드 생성
	else if (line->info[*i] == T_OPERATOR && 
	(line->data[*i] == '>' || line->data[*i] == '<'))
		return (redirection(i, line, arg, flag));	// 리다이렉션 노드 생성
	else
		return (NULL);
}

void	lexicize(t_arg *arg, int flag)	// 왼쪽부터 순서대로 이어진 연결리스트 만들기
{
	t_node	*tmp;
	int		i;

	i = 0;
	if (arg->line.data[0] == '\0')
		return ;
	arg->ast_head = check_type(&arg->line, &i, arg, flag);	// 첫번째 노드 생성
	tmp = arg->ast_head;
	while (tmp && arg->line.info[i] != '\0')
	{
		while (arg->line.info[i] == T_SPACE)
			i++;
		tmp->right = check_type(&arg->line, &i, arg, flag);	// 두번째 노드 생성....
		if (tmp->right)
			tmp->right->left = tmp;
		tmp = tmp->right;
	}
}
