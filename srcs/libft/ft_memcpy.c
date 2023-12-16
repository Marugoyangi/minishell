/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 13:34:34 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/16 15:05:12 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, int n)
{
	int				i;
	unsigned char	*destp;
	unsigned char	*srcp;

	i = 0;
	destp = (unsigned char *)dest;
	srcp = (unsigned char *)src;
	if (dest == 0)
		return (0);
	if (dest == src || src == 0)
		return (dest);
	while (i < n)
	{
		destp[i] = srcp[i];
		i++;
	}
	return (dest);
}
