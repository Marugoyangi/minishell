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

int	is_delimiter(char c)
{
	if (('a' < c && c < 'z') && c == '_' && \
	('A' < c && c < 'Z') && ('0' < c && c < '9'))
		return (0);
	return (1);
}

void	expand_envp_quoted(int *position, t_line *line, t_env *envp_head)
{
	int		i;
	int		start;

	i = position[0];
	while (i < position[1] && line->info[i])
	{
		while (line->info[i] && line->info[i] != T_ENV_QUOTED)
			i++;
		if (line->info[i] && (line->info[i] == T_ENV_QUOTED))
		{
			start = i;
			while (is_delimiter(line->info[i]))
				i++;
			if (start + 1 == i) // $하나만 있는 경우
			{
				ft_delete_line(1, line, start + 1);
				i -= 1;
				position[1] -= 1;
			}
			else
			{
				printf("ft %s \n", ft_substr(line->data, \
				start, i - start));
				i += start + replace_line(find_env(envp_head, ft_substr(line->data, \
				start, i - start)), line, start, i);
				position[1] += ft_strlen(find_env(envp_head, ft_substr(line->data, \
				start,  i - start)));
				printf("position[1] : %d\n", position[1]);
			}
		}
	}
}

void	expand_envp(int *position, t_line *line, t_env *envp_head)
{
	int		i;
	int		start;

	if (envp_head)
		i = 0;
	i = position[0];
	printf("first position[1] : %d\n", position[1]);
	while (i < position[1] && line->info[i])
	{
		while (line->info[i] && line->info[i] != T_ENV)
			i++;
		if (line-> info[i] && (line->info[i] == T_ENV))
		{
			start = i;
			i++;
			while(i < position[1] && line->info[i] != T_SPACE && line->info[i] != T_SINGLE_QUOTE && \
			line->info[i] != T_DOUBLE_QUOTE && line->info[i] != T_ENV)
				i++;
			if (start + 1 == i) // $하나만 있는 경우
			{
				ft_delete_line(1, line, start + 1);
				i -= 1;
				position[1] -= 1;
			}
			else
			{
				printf("i : %d\n", i);
				printf("start : %d\n", start);
				printf("ft %s\n", ft_substr(line->data, \
				start + 1, i - start + 1));
				i += replace_line(find_env(envp_head, \
				ft_substr(line->data, start + 1, i - start + 1)), line, start, i + 1);
				position[1] += start + ft_strlen(find_env(envp_head, ft_substr(line->data, \
				start + 1, i - start + 1)));
				printf("after position[1] : %d\n", position[1]);
			}
		}
	}
}

void printfline(t_line *line)
{
	int i;

	i = 0;
	while (line->info[i])
	{
		printf("[%d]", line->info[i]);
		i++;
	}
	printf("\n");
}

void	expand_vars(int *position, t_line *line, t_arg *arg)
{
	expand_envp(position, line, arg->envp_head);
	expand_envp_quoted(position, line, arg->envp_head);
	expand_tilde(position, line, arg);
	remove_quotes(position, line);
	expand_asterisk(position, line);
}

