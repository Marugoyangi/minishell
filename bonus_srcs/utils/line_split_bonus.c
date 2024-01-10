/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_split_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 05:00:25 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:21:06 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int	ft_store_word(char *word, const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != c && s[i] != '\0')
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
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
		if (s[i] == c && words == word)
			return (i);
		while (s[i] == c)
			i++;
		j = 0;
		while (s[i + j] != c && s[i + j] != '\0')
			j++;
		if (words == word)
			return (word);
		p[word] = (char *)ft_malloc(sizeof (char) * (j + 1));
		word += ft_store_word(p[word], s + i, c);
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
		if (line[i] == -T_ASTERISK)
		{
			check_point = i;
			while (check_point > 0 && line[check_point] != c)
				check_point--;
			if (line[check_point] == c)
				return (check_point + 1);
			else
				return (0);
		}
		i++;
	}
	return (0);
}

char	**line_split(char *line, char c)
{
	int		words;
	char	**p;
	int		i;
	int		prefix;

	prefix = get_prefix_pwd(line, c);
	words = ft_count_words(&line[prefix], c);
	p = (char **)ft_malloc(sizeof (char *) * (words + 3));
	if (prefix == 0 && words == 0)
	{
		p[0] = NULL;
		return (p);
	}
	p[0] = NULL;
	if (prefix != 0)
		p[0] = ft_substr(line, 0, prefix);
	i = ft_alloc_words(p + 2, &line[prefix], c, words);
	if (i != -1)
		p[1] = ft_strdup(&line[prefix + i]);
	else
		p[1] = ft_strdup("");
	p[words + 2] = NULL;
	return (p);
}
