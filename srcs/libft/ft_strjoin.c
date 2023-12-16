/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 00:36:41 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/03/15 00:36:41 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		s1_len;
	int		s2_len;
	char	*p;
	int		i;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	p = (char *)malloc(sizeof (char) * (s1_len + s2_len + 1));
	if (p == 0)
		return (0);
	i = 0;
	while (i < s1_len)
	{
		p[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < s2_len)
	{
		p[s1_len + i] = s2[i];
		i++;
	}
	p[s1_len + i] = '\0';
	return (p);
}
