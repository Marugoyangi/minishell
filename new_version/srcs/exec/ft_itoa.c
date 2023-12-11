/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 21:49:38 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/11 14:10:44 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_itoa(int n);
int		ft_countdigit(int n);
void	ft_nbrdup(int n, int digit, char *nbr);

// char	*ft_itoa(int n)
// {
// 	char	*nbr;
// 	int		digit;

// 	digit = ft_countdigit(n);
// 	if (n == 0)
// 		return (ft_strdup("0"));
// 	nbr = (char *)malloc(sizeof(char) * (digit + 1));
// 	if (nbr == 0)
// 		return (0);
// 	ft_nbrdup(n, digit + 1, nbr);
// 	nbr[0] = '.';
// 	nbr[digit] = 0;
// 	return (nbr);
// }

// int	ft_countdigit(int n)
// {
// 	int	digit;

// 	digit = 0;
// 	while (n > 0)
// 	{
// 		n /= 10;
// 		digit += 1;
// 	}
// 	return (digit);
// }

// void	mft_nbrdup(int n, int digit, char *nbr)
// {
// 	if (n > 0)
// 	{
// 		nbr[digit - 1] = n % 10 + '0';
// 		ft_nbrdup(n / 10, digit - 1, nbr);
// 	}
// }
