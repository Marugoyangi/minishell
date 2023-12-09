/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 14:46:47 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/07 14:46:47 by jeongbpa         ###   ########.fr       */
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
		printf("node->data->after split : %s\n", node->data);
	}
	while (node->line && node->line->info[i])
	{
		while (node->line->info[i] == T_SPACE && node->line->info[i] != '\0')
			i++;
		k = i;
		while (node->line->info[i] && node->line->info[i] != T_SPACE)
			i++;
		if (i > k)
			argv[word++] = ft_substr(node->line->data, k, i - k);
	}
}

void	split_expanded(t_node *node)
{
	char	**argv;
	int		i;
	int		words;

	i = 0;
	words = 0;
	while (node->line && node->line->info && (node->line->info[i] == T_OPERATOR || \
	node->line->info[i] == T_SPACE))
		i++;
	if (i != 0 && node->line->info[i - 1] == T_OPERATOR)
		words = 1;
	while (node->line && node->line->info && node->line->info[i])
	{
		while (node->line->info[i] != T_SPACE && node->line->info[i] != '\0')
			i++;
		if (node->line->info[i] == T_SPACE || node->line->info[i] == '\0')
			words++;
		while (node->line->info[i] == T_SPACE)
			i++;
	}
	argv = malloc(sizeof(char *) * (words + 1));
	argv[words] = NULL;
	alloc_expanded(node, argv);
	node->argv = argv;
	if (node->type == L_SIMPLE_COMMAND)
		node->data = ft_strdup(node->argv[0]);
}

int	node_type_numbers(t_node *root, int type)
{
	t_node	*tmp;
	int		i;
	int		j;

	tmp = root;
	i = 0;
	if (type == L_SIMPLE_COMMAND)
	{
		while (tmp)
		{
			if (tmp->type == L_SIMPLE_COMMAND)
			{
				j = 0;
				while (tmp->argv[j])
					j++;
			}
			i += j;
			tmp = tmp->right;
		}
		return (i);
	}
	while (tmp)
	{
		if (tmp->type == type)
			i++;
		tmp = tmp->right;
	}
	return (i);
}

void	sort_free(t_node **node)
{
	int	i;

	i = 0;
	if (node == NULL)
		return ;
	while (node[i])
	{
		if (node[i]->data)
			free(node[i]->data);
		node[i]->data = NULL;
		free(node[i]);
		node[i] = NULL;
 		i++;
	}
	free (node);
}

t_node	*sort_redirection(t_node **node)
{
	int	i;
	int	last;

	i = 0;
	if (node == NULL)
		return (NULL);
	while (node[i])
		i++;
	last = --i;
	while (node[i] && i > 0)
	{
		node[i]->left = node[i - 1];
		node[i]->right = NULL;
		i--;
	}
	node[0]->left = NULL;
	node[0]->right = NULL;
	node[last]->right = NULL;
	return (node[last]);
}

t_node	**sort_node(t_node *root, int type)
{
	t_node	**tmp;
	t_node	*node;
	int	i;

	if (node_type_numbers(root, type) == 0)
		return (NULL);
	node = root;
	tmp = malloc(sizeof(t_node *) * (node_type_numbers(root, type) + 1));
	i = 0;
	while (node)
	{
		if (node->type == type)
		{
			tmp[i] = node;
			i++;
		}
		node = node->right;
	}
	tmp[node_type_numbers(root, type)] = NULL;
	return (tmp);
}

void	append_subshell(t_node *root)
{
	t_node	*tmp;
	t_node	**red;
	char	*red_str;
	int		i;

	i = 0;
	tmp = root;
	if (tmp == NULL)
		return ;
	red_str = ft_strdup("");
	red = sort_node(tmp, L_REDIRECTION);
	if (red == NULL)
		return ;
	while (red[i])
	{
		red_str = ft_strjoin(red_str, red[i]->data);
		red_str = ft_strjoin(red_str, red[i]->argv[0]);
		i++;
	}
	i = 0;
	red_str = ft_strjoin(ft_substr(root->data, 0, 1), red_str);
	red_str = ft_strjoin(red_str, " ");
	red_str = ft_strjoin(red_str, ft_substr(root->data, 1, ft_strlen(root->data) - 1));
	root->data = ft_strdup(red_str);
	free (red_str);
	sort_free(red);
}

t_node *append_cmd(t_node *root, int type)
{
	t_node	*node;
	t_node	*result;
	char	**tmp;
	int		i;
	int		j;

	node = root;
	if (node_type_numbers(root, type) == 0)
		return (NULL);
	tmp = malloc((node_type_numbers(root, type) + 1) * sizeof(t_node));
	result = create_node(NULL, NULL, type);
	i = 0;
	while (node)
	{
		if (node->type == type)
		{
			j = 0;
			while (node->argv[j])
			{
				tmp[i] = node->argv[j];
				i++;
				j++;
			}
		}
		node = node->right;
	}
	tmp[i] = NULL;
	result->data = ft_strdup(tmp[0]);
	result->argv = tmp;
	return (result);
}