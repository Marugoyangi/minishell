/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 01:02:49 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/03/15 01:02:49 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_malloc(int start, int end)
{
	char	*str;

	if (end <= start)
	{
		str = (char *)malloc(sizeof (char) * 1);
		if (str == 0)
			return (0);
		str[0] = '\0';
		return (str);
	}
	str = (char *)malloc(sizeof (char) * (end - start + 1));
	if (str == 0)
		return (0);
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	start;
	int	end;
	char	*str;

	if (set == 0)
		return (ft_strdup(s1));
	start = 0;
	end = ft_strlen(s1);
	while (s1[start])
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
	str = ft_malloc(start, end);
	if (str != 0)
		ft_strlcpy(str, s1 + start, end - start + 1);
	return (str);
}
