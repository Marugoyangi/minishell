/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_sort_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:23:38 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:23:38 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_node	*sort_redirection(t_node **node)
{
	int	i;

	i = 0;
	if (node == NULL)
		return (NULL);
	while (node[i] && node[i + 1])
	{
		node[i]->left = node[i + 1];
		node[i]->right = NULL;
		i++;
	}
	if (node[i])
	{
		node[i]->left = NULL;
		node[i]->right = NULL;
	}
	return (node[0]);
}

t_node	**sort_node(t_node *root, int type)
{
	t_node	**tmp;
	t_node	*node;
	int		i;

	if (root == NULL)
		return (NULL);
	node = root;
	tmp = ft_malloc(sizeof(t_node *) * (node_type_numbers(root, type) + 1));
	tmp = ft_memset(tmp, 0, sizeof(t_node *) * \
	(node_type_numbers(root, type) + 1));
	tmp[node_type_numbers(root, type)] = NULL;
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
	return (tmp);
}

void	append_subshell(t_node *root)
{
	t_node	**red;
	char	*str;
	int		i;

	i = -1;
	red = sort_node(root, L_REDIRECTION);
	if (red == NULL)
		return ;
	str = ft_strdup("");
	while (red[++i])
	{
		str = modified_strjoin(str, red[i]->data, 1);
		str = modified_strjoin(str, red[i]->argv[0], 1);
	}
	str = modified_strjoin(ft_substr(root->data, 0, 1), str, 0);
	str = modified_strjoin(str, " ", 1);
	str = modified_strjoin(str, ft_substr(root->data, 1, \
	ft_strlen(root->data) - 1), 0);
	free(root->data);
	root->data = modified_strdup(str);
	sort_free(red);
}

void	append_cmd_iter(t_node *node, char **tmp, int type)
{
	int		i;
	int		j;

	i = 0;
	while (node)
	{
		if (node->type == type)
		{
			j = 0;
			if (!node->argv && node->line)
				tmp[i] = ft_strdup(node->line->data);
			else
			{
				while (node->argv && node->argv[j])
				{
					tmp[i] = ft_strdup(node->argv[j]);
					i++;
					j++;
				}
			}
		}
		node = node->right;
	}
}

t_node	*append_cmd(t_node *root, int type)
{
	t_node	*result;
	t_node	*node;
	char	**tmp;

	if (root == NULL)
		return (NULL);
	node = root;
	if (node_type_numbers(root, type) == 0)
		return (NULL);
	tmp = ft_calloc(sizeof(char *), (node_type_numbers(root, type) + 1));
	tmp[node_type_numbers(root, type)] = NULL;
	result = create_node(NULL, NULL, type);
	append_cmd_iter(node, tmp, type);
	if (tmp[0])
		result->data = ft_strdup(tmp[0]);
	result->line = NULL;
	result->argv = tmp;
	return (result);
}
