/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 05:51:56 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/02 05:51:56 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quotes(int *position, t_line *line)
{
	int		i;
	int		start;
	int		end;

	i = position[0];
	while (line->info[i])
	{
		while (line->info[i] && line->info[i] != T_SINGLE_QUOTE && \
		line->info[i] != T_DOUBLE_QUOTE)
			i++;
		if (line->info[i] && (line->info[i] == T_SINGLE_QUOTE || \
		line->info[i] == T_DOUBLE_QUOTE))
		{
			start = i;
			while (line->info[i] && (line->info[i] == T_SINGLE_QUOTE || \
			line->info[i] == T_DOUBLE_QUOTE))
				i++;
			end = i;
			i += replace_line(NULL, line, start, end);
		}
	}
	position[1] = i;
}

int	is_delimiter(char c)
{
	if (('a' < c && c < 'z') && c == '_' && \
	('A' < c && c < 'Z') && ('0' < c && c < '9'))
		return (0);
	return (1);
}

void	expand_envp_quoted(int *position, t_line *line, t_node *envp_head)
{
	int		i;
	int		start;
	int		end;

	if (envp_head)  // 삭제
		i = 0;
	i = position[0];
	while (line->info[i])
	{
		while (line->info[i] && line->info[i] != T_ENV_QUOTED)
			i++;
		if (line->info[i] && (line->info[i] == T_ENV_QUOTED))
		{
			start = i;
			while (!is_delimiter(line->info[i]))
				i++;
			if (start == i) // $하나만 있는 경우
				i += replace_line(NULL, line, start, i);
			else
			{
				end = i;
				// i += replace_line(find_envp_value(envp_head, line, start, end) \
				// , line, start, end);
				i += replace_line(getenv(ft_substr(line->data, start + 1, end - start - 1)), line, start, end);
			}
		}
	}
	position[1] = i;
}

void	expand_envp(int *position, t_line *line, t_node *envp_head)
{
	int		i;
	int		start;
	int		end;

	if (envp_head == NULL || envp_head->data == NULL)	//삭제
		i = 0;
	i = position[0];
	while (line->info[i])
	{
		while (line->info[i] && line->info[i] != T_ENV)
			i++;
		if (line-> info[i] && (line->info[i] == T_ENV))
		{
			start = i;
			while(line->info[i] != T_SPACE && line->info[i] != T_SINGLE_QUOTE && \
			line->info[i] != T_DOUBLE_QUOTE && line->info[i] != T_ENV)
				i++;
			if (start == i) // $하나만 있는 경우
				replace_line(NULL, line, start, i);
			else
			{
				end = i;
				// i += replace_line(find_envp_value(envp_head, line, start, end)\
				// , line, start, end);
				i += replace_line(getenv(ft_substr(line->data, start + 1, end - start - 1)), line, start, end);
			}
		}
	}
	position[1] = i;
}

void	expand_vars(int *position, t_line *line, t_arg *arg)
{
	int		i;

	i = position[0];
	expand_envp(position, line, arg->envp_head);
	expand_envp_quoted(position, line, arg->envp_head);
	remove_quotes(position, line);
	expand_tilde(position, line, arg);
	expand_asterisk(position, line);
	printf("final line->data : %s\n", line->data);
}
