/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 05:49:41 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/03/15 05:49:41 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_isintmin(char *p)
{
	p = (char *)malloc(sizeof (char) * 12);
	if (p == 0)
		return (0);
	ft_strlcpy(p, "-2147483648", 12);
	return (p);
}

static int	ft_numlen(int n)
{
	int	i;

	i = 0;
	if (n <= 0)
		i++;
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
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
