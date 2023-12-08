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

void	split_red(t_node *node)
{
	char	**tmp;
	char	*str;
	char	*red;
	int		i;

	i = 0;
	while (node->data[i] == '>' || node->data[i] == '<')
		i++;
	str = ft_substr(node->data, i, ft_strlen(node->data) - i);
	if (ft_strlen(str) == 0)
	{
		free(str);
		str = NULL;
		return ;
	}
	red = ft_substr(node->data, 0, i);
	tmp = malloc(sizeof(char *) * 2);
	tmp[1] = NULL;
	tmp[0] = ft_strtrim(ft_strdup(str), " ");
	free(str);
	str = NULL;
	free(node->data);
	node->data = ft_strdup(red);
	node->argv = tmp;
}

int	node_type_numbers(t_node *root, int type)
{
	t_node	*tmp;
	int		i;

	tmp = root;
	i = 0;
	while (tmp)
	{
		if (tmp->type == type)
			i++;
		tmp = tmp->right;
	}
	printf("node_type_numbers: %d\n", i);
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

	node = root;
	if (node_type_numbers(root, type) == 0)
		return (NULL);
	tmp = malloc((node_type_numbers(root, type) + 1) * sizeof(t_node));
	result = create_node(NULL, type, NULL);
	i = 0;
	while (node)
	{
		if (node->type == type && result->data == NULL)
			result->data = ft_strdup(node->data);
		else if (node->type == type && result->data != NULL)
		{
			tmp[i] = ft_strdup(node->data);
			i++;
		}
		node = node->right;
	}
	tmp[i] = NULL;
	result->argv = tmp;
	return (result);
}