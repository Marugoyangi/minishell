/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexicize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 03:10:34 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/11/30 07:26:18 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*redirection(int *i, t_line *line)
{
	int		position[2];
	char	redirection;

	position[0] = *i;
	redirection = line->data[*i];
	*i = *i + 1;
	if (line->data[*i] == redirection)
		redirection = redirection * -1;
	while (line->info[*i] == T_SPACE)
		(*i)++;
	while (line->info[*i] == T_DOUBLE_QUOTE || line->info[*i] == T_SINGLE_QUOTE \
	|| line->info[*i] == T_WORD || line->info[*i] == T_LITERAL)
		(*i)++;
	position[1] = *i;
	*i = position[0];
	// position[1] = expand_vars(position, line); // 아스테리스크, 환경변수, 틸더, 쿼트 처리/제거 // 종료지점을 체크한다음 거기서부터 들어가서 확장
	return (create_node(ft_substr(line->data, position[0], \
	position[1] - position[0]), L_REDIRECTION, i));
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

t_node	*simple_cmd(int *i, t_line *line)
{
	int		position[2];

	position[0] = *i;
	while (line->info[*i] == T_WORD || line->info[*i] == T_LITERAL || \
	line->info[*i] == T_SPACE || line->info[*i] == T_DOUBLE_QUOTE || \
	line->info[*i] == T_SINGLE_QUOTE || line->info[*i] == T_ENV || \
	line->info[*i] == T_ENV_QUOTED)
		(*i)++;
	position[1] = *i;
	*i = position[0];
	// position[1] = expand_vars(position, line); // 아스테리스크, 환경변수, 틸더, 쿼트 처리/제거 // 종료지점을 체크한다음 거기서부터 들어가서 확장
	return (create_node(ft_substr(line->data, position[0], \
	position[1] - position[0]), L_SIMPLE_COMMAND, i));
}

t_node	*check_type(t_line *line, int *i)	// 타입 확인 후 노드 생성
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
	|| line->info[*i] == T_DOUBLE_QUOTE || line->info[*i] == T_SINGLE_QUOTE)
		return (simple_cmd(i, line));	// 단순 명령어 노드 생성
	else if (line->info[*i] == T_PARENTHESIS)
		return (subshell(i, line));	// 서브셸 노드 생성
	else if (line->info[*i] == T_OPERATOR && \
	(line->data[*i] == '>' || line->data[*i] == '<'))
		return (redirection(i, line));	// 리다이렉션 노드 생성
	else
		return (NULL);
}

void	lexicize(t_arg *arg)	// 왼쪽부터 순서대로 이어진 연결리스트 만들기
{
	t_node	*tmp;
	int		i;

	i = 0;
	if (arg->input.data[0] == '\0')
		return ;
	arg->ast_head = check_type(&arg->input, &i);	// 첫번째 노드 생성
	tmp = arg->ast_head;
	while (tmp && arg->input.data[i] != '\0')
	{
		while (arg->input.info[i] == T_SPACE)
			i++;
	// &가 하나일 때 문자로 변경(과제에서 제외된 기능)
		tmp->right = check_type(&arg->input, &i);	// 두번째 노드 생성....
		tmp->right->left = tmp;
		tmp = tmp->right;
	}
}
