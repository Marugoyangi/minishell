/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 05:51:56 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/10 07:36:53 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_number(char c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}

int	is_not_delimiter(char c)
{
	if (('a' <= c && c <= 'z') || c == '_' || ('A' <= c && c <= 'Z'))
		return (1);
	return (0);
}

t_line	*get_line_info(char *env)
{
	t_line	*line;

	line = (t_line *)malloc(sizeof(t_line));
	line->data = ft_strdup(env);
	line->info = NULL;
	tokenize(line);
	return (line);
}

void	expand_node(t_line *line, t_arg *arg, int *i, int *start)
{
	t_line	*env_line;
	char	*env;

	while (line->info[*i] && (is_not_delimiter(line->data[*i]) || \
		is_number(line->data[*i])))
		(*i)++;
	if (*start != -1)
	{
		printf("env before: %s\n", line->data);
		env = ft_substr(line->data, *start + 1, *i - *start - 1);
		printf("env: %s\n", env);
		if (!ft_strcmp(env, "?"))
			env = ft_itoa(arg->last_exit_status);
		else
			env = find_env(arg->envp_head, env);
		if (!env)
		{
			ft_delete_line(*i -  *start, line, *i);
			*i -= *i - *start;
			return ;
		}
		env_line = get_line_info(env);
		*i += replace_line(env_line, line, *start, *i) - (*i - *start);
		printf("i: %d\n", *i);
	}
}

void	expand_envp(t_line *line, t_arg *arg)
{

	int		i;
	int		start;

	i = 0;
	while (line && line->info && line->info[i])
	{
		while (line->info[i] && line->info[i] != T_ENV)
			i++;
		while (line->info[i] && (line->info[i] == T_ENV))
			i++;
		if (i > 0 && line->info[i - 1] == T_ENV && line->info[i])
			start = i - 1;
		if (i > 0 && line->info[i - 1] == T_ENV && !is_number(line->data[i]) \
			&& line->data[i] != '\0')
			i++;
		else
			start = - 1;
		expand_node(line, arg, &i, &start);
		start = -1;
	}
}

void	printfline(t_node *node)
{
	while (node)
	{
		if (node)
		printf("expand node->data: %s\n", node->data);
		if (node->line)
		{
			printf("expand line->data: %s\n", node->line->data);
			int i = 0;
			while (node->line->info[i])
			printf("[%d]", node->line->info[i++]);
			printf("\n");
		}
		int k = 0;
		while (node->line && node->argv && node->argv[k])
		{
			printf("expand argv[%d]: %s\n", k, node->argv[k]);
			k++;
		}
		node = node->right;
	}
	printf("\n");
}

void	expand_vars(t_arg *arg)
{
	t_node	*tmp;

	tmp = arg->ast_head;
	while (tmp)
	{
		expand_envp(tmp->line, arg);
		expand_tilde(tmp->line, arg);
		remove_quotes(tmp->line);
		expand_asterisk(tmp->line);
		split_expanded(tmp);
		tmp = tmp->right;
	}
	printfline(arg->ast_head);
}
