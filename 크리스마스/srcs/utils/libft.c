/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 07:03:07 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/15 14:59:35 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	i = 0;
	if (to_find[0] == 0)
		return (0);
	while (str[i])
	{
		j = 0;
		while (str[i + j] && str[i + j] == to_find[j])
		{
			if (to_find[j + 1] == 0)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	ft_numlen(int n)
{
	int	i;

	i = 1;
	while (n / 10 != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}
static char	*ft_isintmin(char *p)
{
	p = (char *)malloc(sizeof (char) * 12);
	if (p == 0)
		return (0);
	ft_strlcpy(p, "-2147483648", 12);
	return (p);
}

char	*ft_itoa(int n)
{
	char	*p;
	int		len;
	int		end;

	p = 0;
	if (n == -2147483648)
		return (ft_isintmin(p));
	len = ft_numlen(n);
	end = 0;
	p = (char *)malloc(sizeof (char) * (len + 1));
	if (p == 0)
		return (0);
	p[len++] = '\0';
	if (n < 0)
	{
		p[0] = '-';
		n = -n;
		end = 1;
	}
	while (--len > end)
	{
		p[len - 1] = n % 10 + '0';
		n = n / 10;
	}
	return (p);
}


// char	*modified_strjoin(char *s1, char *s2, int flag)
// {
// 	char	*str;
// 	int		i;
// 	int		k;

// 	if (s1 == 0 || s2 == 0)
// 		return (0);
// 	str = (char *)ft_malloc(sizeof (char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
// 	if (str == 0)
// 		return (0);
// 	i = 0;
// 	while (s1[i])
// 	{
// 		str[i] = s1[i];
// 		i++;
// 	}
// 	k = 0;
// 	while (s2[k])
// 	{
// 		str[i + k] = s2[k];
// 		k++;
// 	}
// 	str[i + k] = 0;
// 	if (flag == 1)
// 		free(s1);
// 	else if (flag == 2)
// 		free(s2);
// 	else if (flag == 0)
// 	{
// 		free(s1);
// 		free(s2);
// 	}
// 	return (str);
// }

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		k;

	if (s1 == 0 && s2 != 0)
		return (ft_strdup((char *)s2));
	if (s2 == 0 && s1 != 0)
		return (ft_strdup((char *)s1));
	str = (char *)ft_malloc(sizeof (char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (str == 0)
		return (0);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	k = 0;
	while (s2[k])
	{
		str[i + k] = s2[k];
		k++;
	}
	str[i + k] = 0;
	return (str);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	if (s1 == NULL || s2 == NULL)
		return (1);
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

// // char	*modified_substr(char *s, int start, int len)
// {
// 	char	*str;
// 	int		i;

// 	if (s == 0)
// 		return (0);
// 	if (start >= ft_strlen(s))
// 		return (ft_strdup(""));
// 	if (len > ft_strlen(s) - start)
// 		len = ft_strlen(s) - start;
// 	str = (char *)ft_malloc(sizeof (char) * (len + 1));
// 	if (str == 0)
// 		return (0);
// 	i = 0;
// 	while (i < len && s[start + i])
// 	{
// 		str[i] = s[start + i];
// 		i++;
// 	}
// 	str[i] = 0;
// 	free(s);
// 	return (str);
// }

char	*ft_substr(char const *s, int start, int len)
{
	char	*str;
	int		i;

	if (s == 0)
		return (0);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	str = (char *)ft_malloc(sizeof (char) * (len + 1));
	if (str == 0)
		return (0);
	i = 0;
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = 0;
	return (str);
}
int ft_strlcpy(char *dest, const char *src, int size)
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


int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char *ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != (char)c && s[i])
		i++;
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (0);
}

char	*ft_strtrim(char *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*str;

	if (set == 0)
		return ((char *)s1);
	start = 0;
	end = ft_strlen(s1);
	while (s1 && s1[start])
	{
		if (!ft_strchr(set, s1[start]))
			break ;
		start++;
	}
	while (end > 0)
	{
		if (!ft_strchr(set, s1[end - 1]))
			break ;
		end--;
	}
	str = ft_malloc(end - start + 1);
	if (str != 0)
		ft_strlcpy(str, s1 + start, end - start + 1);
	return (str);
}


// char	*modified_strdup(char *s)
// {
// 	char	*new_s;
// 	int		i;

// 	i = 0;
// 	if (s == 0)
// 		return (0);
// 	new_s = (char *)ft_malloc(sizeof(char) * (ft_strlen(s) + 1));
// 	if (new_s == 0)
// 		return (0);
// 	while (s[i])
// 	{
// 		new_s[i] = s[i];
// 		i++;
// 	}
// 	new_s[i] = s[i];
// 	free(s);
// 	return (new_s);
// }

char	*ft_strdup(char *s)
{
	char	*new_s;
	int		i;

	i = 0;
	if (s == 0)
		return (0);
	new_s = (char *)ft_malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (new_s == 0)
		return (0);
	while (s[i])
	{
		new_s[i] = s[i];
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}

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
		p[word] = (char *)ft_malloc(sizeof (char) * (j + 1));
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
	p = (char **)ft_malloc(sizeof (char *) * (words + 1));
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
