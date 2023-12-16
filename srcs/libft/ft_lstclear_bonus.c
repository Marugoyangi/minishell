/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:38:32 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/03/20 15:38:32 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (lst == 0 || *lst == 0 || del == 0)
		return ;
	while (*lst != 0)
	{
		temp = (*lst)-> next;
		del((*lst)-> content);
		free(*lst);
		*lst = temp;
	}
}
