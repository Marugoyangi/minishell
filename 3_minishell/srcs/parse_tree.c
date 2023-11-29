/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 09:18:24 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/11/26 22:02:49 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_redirection_list(t_node *root)
{
	if (root != NULL)
	{
		if (root->type == REDIRECTION_LIST)
		{
			parse_redirection_list(root->left);
			parse_redirection(root->right);
		}
	}
}

void	parse_command(t_node *root)
{
	if (root != NULL)
	{
		if (root->type == COMMAND)
		{
			parse_redirection_list(root->left);
			parse_simple_command(root->right);
		}
	}
}

void	parse_pipeline(t_node *root)
{
	if (root != NULL)
	{
		if (root->type == PIPELINE)
		{
			parse_pipeline(root->left);
			parse_command(root->right);
		}
		else
			parse_command(root);
	}
}

void	parse_command_line(t_node *root)
{
	if (root != NULL)
	{
		printf("data : %s\n", root->data);
		if (root->type == COMMAND_LINE)
		{
			parse_pipeline(root->left);
			parse_command_line(root->right);
		}
		else
			parse_pipeline(root);
	}
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strlcpy(char *dest, char *src, int size)
{
	int	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (i < size - 1 && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (ft_strlen(src));
}

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (str1[i] != str2[i] || str1[i] == '\0' || str2[i] == '\0')
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

char **ft_split(char *s, char c)
{
	char	**result;
	int		i;
	int		j;
	int		k;

	if (!s)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (ft_strlen(s) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			k = 0;
			while (s[i + k] && s[i + k] != c)
				k++;
			result[j] = (char *)malloc(sizeof(char) * (k + 1));
			if (!result[j])
				return (NULL);
			ft_strlcpy(result[j], s + i, k + 1);
			i += k;
			j++;
		}
	}
	result[j] = NULL;
	return (result);
}

void	print_node(t_node *root)			// 노드 출력 함수
{
	if (root != NULL) {
		printf("token : %s\n", root->data);
		if (root->left != NULL)
			printf("left : %s\n", root->left->data);
		if (root->right != NULL)
			printf("right : %s\n", root->right->data);
		print_node(root->left);
		print_node(root->right);
	}
}


void	preorder(t_node *root)			// 전위 순회
{
	if (root != NULL) {
		printf("[%s] ", root->data);
		preorder(root->left);
		preorder(root->right);
	}
}

t_node	*create_node(char *token, int type)		// 노드 생성 함수
{
	t_node	*new;

	new = (t_node *)malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->data = token;
	new->type = type;
	new->left = NULL;
	new->right = NULL;
	return (new);
}


char a[] = "cmd1 | cmd2 > file1 < file2 >> file3 && && cmd3 || cmd4 | (cmd5 && cmd6)";

int main()
{
	t_node	*root;
	t_node	*tmp;
	char	**token;
	int		i;

	token = ft_split(a, ' ');
	i = 0;
	while (token[i])
	{
		printf("%s\n", token[i]);
		i++;
	}
	root = create_node(token[0]);
	tmp = root;
	i = 1;
	while (token[i])
	{
	
	}
	preorder(root);
	return (0);
}
