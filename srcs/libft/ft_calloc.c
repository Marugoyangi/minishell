/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 23:25:15 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/03/14 23:25:15 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(int nmemb, int size)
{
	void	*p;

	p = malloc(nmemb * size);
	if (p == 0)
		return (0);
	ft_bzero(p, nmemb * size);
	return (p);
}
