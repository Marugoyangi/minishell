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


int	node_type_numbers(t_node *root, int type)
{
	t_node	*tmp;
	int		i;
	int		j;

	tmp = root;
	i = 0;
	while (tmp)
	{
		if (tmp->type == L_SIMPLE_COMMAND && type == L_SIMPLE_COMMAND)
		{
			j = 1;
			while (tmp->argv && tmp->argv[j - 1])
				j++;
			j -= 1;
			i += j;
		}
		else if (tmp->type == type)
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
		if (node[i]->argv)
			free_split((void **)node[i]->argv);
		if (node[i]->line)
		{
			free(node[i]->line->data);
			free(node[i]->line->info);
			free(node[i]->line);
		}
		node[i]->line = NULL;
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
	tmp = calloc(sizeof(t_node *), (node_type_numbers(root, type) + 1)); //ft로 바꿀것
	if (tmp == NULL)
		return (NULL);
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
		red_str = modified_strjoin(red_str, red[i]->data, 1);
		red_str = modified_strjoin(red_str, red[i]->argv[0], 1);
		i++;
	}
	i = 0;
	red_str = modified_strjoin(ft_substr(root->data, 0, 1), red_str, 0);
	red_str = modified_strjoin(red_str, " ", 1);
	red_str = modified_strjoin(red_str, ft_substr(root->data, 1, ft_strlen(root->data) - 1), 2);
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
	tmp = malloc (sizeof(char *) * (node_type_numbers(root, type) + 1));
	if (tmp == NULL)
		return (NULL);
	tmp [node_type_numbers(root, type)] = NULL;
	result = create_node(NULL, NULL, type);
	if (result == NULL)
		return (NULL);
	i = 0;
	while (node)
	{
		if (node->type == type)
		{
			j = 0;
			while (node->argv && node->argv[j])
			{
				tmp[i] = ft_strdup(node->argv[j]);
				i++;
				j++;
			}
		}
		node = node->right;
	}
	result->data = ft_strdup(tmp[0]);
	result->line = NULL;
	result->argv = tmp;
	return (result);
}