/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 21:49:38 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/09 02:39:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_itoa(int n);
int		ft_countdigit(int n);
void	ft_nbrdup(int n, int digit, char *nbr);

char	*ft_itoa(int n)
{
	char	*nbr;
	int		digit;

	digit = ft_countdigit(n);
	nbr = (char *)malloc(sizeof(char) * (digit + 1));
	if (nbr == 0)
		return (0);
	ft_nbrdup(n, digit + 1, nbr);
	nbr[0] = '.';
	nbr[digit] = 0;
	return (nbr);
}

int	ft_countdigit(int n)
{
	int	digit;

	digit = 0;
	while (n > 0)
	{
		n /= 10;
		digit += 1;
	}
	return (digit);
}

void	ft_nbrdup(int n, int digit, char *nbr)
{
	if (n > 0)
	{
		nbr[digit - 1] = n % 10 + '0';
		ft_nbrdup(n / 10, digit - 1, nbr);
	}
}