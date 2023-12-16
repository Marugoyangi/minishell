/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 00:10:30 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/03/15 00:10:30 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, int start, int len)
{
	int		i;
	int		real_len;
	char	*substr;

	i = 0;
	if (ft_strlen(s) < start)
		real_len = 0;
	else if (ft_strlen(s + start) > len)
		real_len = len;
	else
		real_len = ft_strlen(s + start);
	substr = (char *)malloc(sizeof (char) * (real_len + 1));
	if (substr == 0)
		return (0);
	while (i < real_len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}
