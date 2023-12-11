/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 05:51:56 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/11 14:19:52 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	alloc_expanded(t_node *node, char **argv)
{
	int		i;
	int		j;
	int		k;
	int		word;

	i = 0;
	j = 0;
	word = 0;
	while (node->line && node->line->info && (node->line->info[i] == T_OPERATOR || node->line->info[i] == T_SPACE)
		&& node->line->info[i] != '\0')
			i++;
	if (i != 0 && (node->line->info[i - 1] == T_OPERATOR || \
		node->line->info[i - 1] == T_SPACE))
	{
		j = 0;
		free (node->data);
		node->data = malloc(sizeof(char) * (i + 1));
		while (j < i)
		{
			node->data[j] = node->line->data[j];
			j++;
		}
		node->data[j] = '\0';
	}
	while (node->line && node->line->info[i])
	{
		while (node->line->info[i] == T_SPACE && node->line->info[i] != '\0')
			i++;
		k = i;
		while (node->line->info[i] && node->line->info[i] != T_SPACE
		&& node->line->info[i] != '\0')
			i++;
		if (i > k)
			argv[word++] = ft_substr(node->line->data, k, i - k); //여기서부터
	}
}

void	split_expanded(t_node *node)
{
	char	**argv;
	int		i;
	int		words;

	i = 0;
	words = 0;
	printf("split_expanded node->data" "[%s]\n", node->data);
	while (node->line && node->line->info && node->line->data && (node->line->info[i] == T_OPERATOR || \
	node->line->info[i] == T_SPACE))
		i++;
	while (node->line && node->line->info && node->line->info[i])
	{
		while (node->line->info[i] != T_SPACE && node->line->info[i] != '\0')
			i++;
		if (node->line->info[i] == T_SPACE || node->line->info[i] == '\0')
			words++;
		while (node->line->info[i] == T_SPACE)
			i++;
	}
	if (words == 0)
	{
		node->argv = NULL;
		return ;
	}
	argv = malloc(sizeof(char *) * (words + 1));
	argv[words] = NULL;
	node->argv = argv;
	alloc_expanded(node, argv);
	if (node->type == L_SIMPLE_COMMAND)
	{
		free(node->data);
		node->data = ft_strdup(argv[0]);
	}
}


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
	ft_free((void *)env);
	return (line);
}

void	expand_node(t_line **line, t_arg *arg, int *i, int *start)
{
	t_line	*env_line;
	char	*env;

	while ((*line)->info[*i] && (is_not_delimiter((*line)->data[*i]) || \
		is_number((*line)->data[*i])))
		(*i)++;
	if (*start != -1)
	{
		env = ft_substr((*line)->data, *start + 1, *i - *start - 1);
		if (!ft_strcmp(env, "?"))
		{	
			free(env);
			env = ft_itoa(arg->last_exit_status);
		}
		else
			env = ft_strdup(find_env(arg->envp_head, env));
		if (!env)
		{
			ft_delete_line(*i -  *start, line, *i);
			*i -= *i - *start;
			return ;
		}
		else
		{
			env_line = get_line_info(env);
			*i += replace_line(env_line, line, *start, *i) - (*i - *start);
		}
	}
}

void	expand_envp(t_line **line, t_arg *arg)
{

	int		i;
	int		start;

	i = 0;
	while ((*line) && (*line)->info && (*line)->info[i])
	{
		while ((*line)->info[i] && (*line)->info[i] != T_ENV)
			i++;
		while ((*line)->info[i] && ((*line)->info[i] == T_ENV))
			i++;
		if (i > 0 && (*line)->info[i - 1] == T_ENV && (*line)->info[i])
			start = i - 1;
		if (i > 0 && (*line)->info[i - 1] == T_ENV && !is_number((*line)->data[i]) \
			&& (*line)->data[i] != '\0')
			i++;
		else
			start = - 1;
		expand_node(line, arg, &i, &start);
		start = -1;
	}
}

void print_expanded(t_arg *arg)
{
	t_node	*tmp;

	tmp = arg->ast_head;
	while (tmp)
	{
		printf("itoa : %s\n", ft_itoa(1));
		printf("data: %s\n", tmp->data);
		int i = 0;
		while(tmp->argv && tmp->argv[i])
		{
			printf("[%s]", tmp->argv[i]);
			i++;
		}
		printf("\n");
		tmp = tmp->right;
	}
}

void	expand_vars(t_arg *arg)
{
	t_node	*tmp;

	tmp = arg->ast_head;
	while (tmp)
	{
		expand_envp(&tmp->line, arg);
		expand_tilde(&tmp->line, arg);
		remove_quotes(&tmp->line);
		expand_asterisk(&tmp->line);
		split_expanded(tmp);
		tmp = tmp->right;
	}
	print_expanded(arg);
}
