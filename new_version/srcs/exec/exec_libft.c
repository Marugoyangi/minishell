/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_libft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 18:54:11 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/10 01:28:08 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	ft_strcmp(const char *s1, const char *s2)
// {
// 	size_t	i;

// 	i = 0;
// 	while (s1[i] == s2[i])
// 	{
// 		if (!s1[i])
// 			return (0);
// 		++i;
// 	}
// 	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
// }

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (s1[i] == s2[i] && i + 1 < n && s1[i] != 0)
		++i;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
// size_t	ft_strlen(const char *s)
// {
// 	size_t	str_len;

// 	str_len = 0;
// 	while (s[str_len] != 0)
// 		++str_len;
// 	return (str_len);
// }

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char		*dest;
// 	size_t		str_len;
// 	size_t		i;

// 	str_len = ft_strlen2(s1) + ft_strlen2(s2);
// 	dest = (char *)malloc(sizeof(char) * (str_len + 1));
// 	if (dest == 0)
// 		return (0);
// 	str_len = 0;
// 	while (s1[str_len])
// 	{
// 		dest[str_len] = s1[str_len];
// 		++str_len;
// 	}
// 	i = 0;
// 	while (s2[i])
// 		dest[str_len++] = s2[i++];
// 	dest[str_len] = 0;
// 	return (dest);
// }
