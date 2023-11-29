/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 04:05:48 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/11/26 22:01:39 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_store_word(char *word, const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != c && s[i] != '\0')
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
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
		p[word] = (char *)malloc(sizeof (char) * (j + 1));
		if (p[word] == 0)
			return (word);
		ft_store_word(p[word], s + i, c);
		i += j;
		word++;
	}
	return (-1);
}

static int	ft_count_words(char const *s, char *set)
{
	int	i;
	int	count;
	int	quote[2];

	i = 0;
	count = 0;
	quote[SINGLE] = 0;
	quote[DOUBLE] = 0;
	while (s[i])
	{
		if (s[i] == '\"' && quote[SINGLE] == 0)
			quote[DOUBLE] = 1;
		else if (s[i] == '\'' && quote[SINGLE] == 0)
			quote[SINGLE] = 1;
		else if (s[i] == '\"' && quote[SINGLE] == 1)
			quote[DOUBLE] = 0;
		else if (s[i] == '\'' && quote[DOUBLE] == 1)
			quote[SINGLE] = 0;
		if ((s[i] != ft_strchr(set, s[i]) && s[i] != '\0') \
		&& (s[i + 1] ==  || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

char	**ft_split(char const *s, char *set)
{
	int		words;
	char	**result;
	int		ret;

	words = ft_count_words(s, set);
	result = (char **)malloc(sizeof (char *) * (words + 1));
	if (result == 0)
		return (0);
	if (words == 0)
	{
		result[0] = 0;
		return (result);
	}
	ret = ft_alloc_words(result, s, set, words);
	if (ret != -1)
	{
		while (ret >= 0)
			free (result[ret--]);
		free(result);
		return (0);
	}
	result[words] = 0;
	return (result);
}
