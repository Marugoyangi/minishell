/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 05:00:25 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/03 10:45:24 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_store_word(char *word, const char *s, char c, int flag)
{
	int	i;

	i = 0;
	if (flag == 1)
	{
		while (s[i])
		{
			word[i] = s[i];
			i++;
		}
		word[i] = '\0';
	}
	else
	{
		while (s[i] != c && s[i] != '\0')
		{
			word[i] = s[i];
			i++;
		}
		word[i] = '\0';
	}
	return (1);
}

static int	ft_alloc_words(char **p, char const *s, char c, int words)
{
	int	i;
	int	j;
	int	word;

	i = 0;
	word = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		j = 0;
		while (s[i + j] != c && s[i + j] != '\0')
			j++;
		if (words == word)
			return (-1);
		if (words - 1 == word)
			p[word] = (char *)malloc(sizeof (char) * (j + 2));
		else
			p[word] = (char *)malloc(sizeof (char) * (j + 1));
		if (p[word] == NULL)
			return (word);
		word += ft_store_word(p[word], s + i, c, words - word);
		i += j;
	}
	return (-1);
}

int	ft_count_words(char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

int	get_prefix_pwd(char *line, char c)
{
	int		i;
	int		check_point;

	i = 0;
	while (line[i])
	{
		check_point = i;
		if (line[i] == '.' && line[i + 1] && line[i + 1] != '.')
		{
			i = i + 1;
			while (line && line[i] == -T_ASTERISK)
				i++;
		}
		else if (line[i] == '.' && line[i + 1] && line[i + 1] == '.')
		{
			i = i + 2;
			while (line[i] && line[i] == -T_ASTERISK)
				i++;
		}
		if (line && line[i] != c)
			return (check_point);
		while (line[i] && line[i] == c)
			i++;
	}
	return (0);
}

char	**line_split(char *line, char c)
{
	int		words;
	char	**p;
	int		prefix;

	prefix = get_prefix_pwd(line, c);
	words = ft_count_words(&line[prefix], c);
	p = (char **)malloc(sizeof (char *) * (words + 2));
	if (p == 0)
		return (0);
	if (prefix == 0 && words == 0)
	{
		p[0] = NULL;
		return (p);
	}
	if (prefix != 0)
		p[0] = ft_substr(line, 0, prefix);
	if (ft_alloc_words(p, &line[prefix], c, words) != -1)
	{
		// 에러처리 해야함
	}
	if (prefix != 0)
		p[words + 1] = NULL;
	else
		p[words] = NULL;
	return (p);
}
