/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 04:05:48 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/03/15 04:05:48 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_store_word(char *word, char *s, char c)
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

int	ft_alloc_word(char **p, char *s, char c, int words)
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

int	ft_count_word(char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if ((s[i] != c && s[i] != '\0') \
		&& (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

char	**ft_split(char *s, char c)
{
	int		words;
	char	**p;
	int		ret;

	if (s == 0)
		return (0);
	words = ft_count_word(s, c);
	p = (char **)malloc(sizeof (char *) * (words + 1));
	if (p == 0)
		return (0);
	if (words == 0)
	{
		p[0] = 0;
		return (p);
	}
	ret = ft_alloc_word(p, s, c, words);
	if (ret != -1)
	{
		while (ret >= 0)
			free (p[ret--]);
		free(p);
		return (0);
	}
	p[words] = 0;
	return (p);
}
