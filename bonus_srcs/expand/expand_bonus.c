/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 05:51:56 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:25:18 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	expand_tilde(t_line **line, t_arg *arg)
{
	int		i;
	char	*home;
	t_line	*tilde_line;

	i = 0;
	home = find_env(arg->envp_head, "HOME");
	if (!home)
		home = arg->tilde;
	while ((*line) && (*line)->info && (*line)->info[i])
	{
		if (((*line)->info[i] == T_LITERAL || (*line)->data[i] != '~'))
			i++;
		else if (((*line)->data[i] == '~' && (*line)->info[i] == T_WORD) && \
		((i > 0 && ((*line)->info[i - 1] == T_SPACE)) || i == 0) && \
		(((*line)->info[i + 1] == T_SPACE || (*line)->info[i + 1] == '\0' || \
		(*line)->data[i + 1] == '/')))
		{
			tilde_line = get_info(ft_strdup(home), 0);
			i += replace_line(tilde_line, line, i, i + 1) - 1;
		}
		else
			i++;
	}
}

void	expand_replace(t_line **line, int *i, int *start, char *env)
{
	t_line	*env_line;

	if (*start != -1)
	{
		if ((*line)->info[*start + 1] == T_LITERAL)
			env_line = get_info(env, 1);
		else
			env_line = get_info(env, 0);
		*i += replace_line(env_line, line, *start, *i) - (*i - *start);
	}
}

void	expand_node(t_line **line, t_arg *arg, int *i, int *start)
{
	char	*env;
	char	*tmp;

	while ((*line)->info[*i] && (is_not_delimiter((*line)->data[*i]) || \
		is_number((*line)->data[*i])))
		(*i)++;
	if (*start != -1)
	{
		tmp = ft_substr((*line)->data, *start + 1, *i - *start - 1);
		if (!ft_strcmp(tmp, "?"))
			env = ft_itoa(arg->last_exit_status);
		else
			env = ft_strdup(find_env(arg->envp_head, tmp));
		free(tmp);
		if (!env || (env && !ft_strlen(env)))
		{
			if (!ft_strlen(env) && env)
				free(env);
			ft_delete_line(*i - *start, line, *i, 1);
			*i -= *i - *start;
		}
		else
			expand_replace(line, i, start, env);
	}
}

void	expand_envp(t_line **line, t_arg *arg)
{
	int		i;
	int		start;

	i = 0;
	while (*line && (*line)->info && (*line)->info[i])
	{
		while ((*line)->info[i] && (*line)->info[i] != T_ENV)
			i++;
		while ((*line)->info[i] && ((*line)->info[i] == T_ENV))
			i++;
		if (i > 0 && (*line)->info[i - 1] == T_ENV && (*line)->info[i])
			start = i - 1;
		if (i > 0 && (*line)->info[i - 1] == T_ENV && \
		!is_number((*line)->data[i]) \
			&& (*line)->data[i] != '\0')
			i++;
		else
			start = -1;
		expand_node(line, arg, &i, &start);
		start = -1;
	}
}

void	expand_vars(t_arg *arg)
{
	t_node	*tmp;
	int		i;

	tmp = arg->ast_head;
	while (tmp)
	{
		if (ft_strncmp(tmp->data, "<<", 2))
		{
			expand_envp(&tmp->line, arg);
			expand_tilde(&tmp->line, arg);
			if (tmp->type == L_REDIRECTION)
			{
				i = 0;
				while (tmp->line->info[i] == T_OPERATOR || \
				tmp->line->info[i] == T_SPACE)
					i++;
				expand_asterisk(&tmp->line, i);
			}
			else
				expand_asterisk(&tmp->line, 0);
		}
		remove_quotes(&tmp->line);
		split_expanded(tmp);
		tmp = tmp->right;
	}
}
