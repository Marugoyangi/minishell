/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 13:44:48 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/16 15:05:00 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, int n)
{
	int				i;
	unsigned char	*destp;
	unsigned char	*srcp;

	i = -1;
	destp = (unsigned char *)dest;
	srcp = (unsigned char *)src;
	if (dest == 0)
		return (0);
	if (dest == src || src == 0)
		return (dest);
	if (dest < src)
		while (++i < n)
			destp[i] = srcp[i];
	else
	{
		while (n > 0)
		{
			destp[n - 1] = srcp[n - 1];
			n--;
		}
	}
	return (dest);
}
