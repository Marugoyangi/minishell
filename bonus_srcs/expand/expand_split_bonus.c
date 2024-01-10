/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:10:03 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:25:10 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	alloc_expanded(t_node *node, char **argv, int i)
{
	int		k;
	int		word;

	word = 0;
	while (node->line && node->line->info && node->line->info[i])
	{
		while (node->line->info[i] == T_SPACE && node->line->info[i] != '\0')
			i++;
		k = i;
		while (node->line->info[i] && node->line->info[i] != T_SPACE && \
		node->line->info[i] != '\0')
			i++;
		if (i > k)
			argv[word++] = ft_substr(node->line->data, k, i - k);
	}
	if (node->type == L_SIMPLE_COMMAND)
	{
		free(node->data);
		node->data = ft_strdup(argv[0]);
	}
}

void	split_allocate(t_node *node, int words)
{
	char	**argv;
	int		i;
	int		j;

	i = 0;
	argv = ft_malloc(sizeof(char *) * (words + 1));
	argv[words] = NULL;
	node->argv = argv;
	while (node->line && node->line->info && (node->line->info[i] == T_OPERATOR \
	|| node->line->info[i] == T_SPACE) && node->line->info[i] != '\0')
		i++;
	if (i != 0 && (node->line->info[i - 1] == T_OPERATOR || \
	node->line->info[i - 1] == T_SPACE))
	{
		j = -1;
		free (node->data);
		node->data = ft_malloc(sizeof(char) * (i + 1));
		while (++j < i)
			node->data[j] = node->line->data[j];
		node->data[j] = '\0';
		node->data = modified_strtrim(node->data, " ");
	}
	alloc_expanded(node, argv, i);
}

void	split_expanded(t_node *node)
{
	int		i;
	int		words;

	i = 0;
	words = 0;
	while (node->line && node->line->info && node->line->data \
	&& (node->line->info[i] == T_OPERATOR || node->line->info[i] == T_SPACE))
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
		node->argv = NULL;
	else
		split_allocate(node, words);
}
