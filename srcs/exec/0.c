/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungwok <seungwok@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 18:54:11 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/06 20:26:20 by seungwok         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(const char *s1, const char *s2);
int	ft_strncmp(const char *s1, const char *s2, size_t n);

size_t	ft_strlen2(const char *s)
{
	size_t	str_len;

	str_len = 0;
	while (s[str_len] != 0)
		++str_len;
	return (str_len);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i])
	{
		if (!s1[i])
			return (0);
		++i;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

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
