/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_ctl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 03:56:07 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/03 05:29:40 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	replace_line(char *data, t_line *line, int start, int end) //길이 줄이거나 나누기
{
	int		i;
	int		j;
	int		k;
	char	*new_data;
	char	*new_info;

	i = 0;
	j = 0;
	k = 0;
	new_data = (char *)malloc(sizeof(char) * (ft_strlen(line->data) + \
	ft_strlen(data) + 1));
	new_info = (char *)malloc(sizeof(char) * (ft_strlen(line->info) + \
	ft_strlen(data) + 1));
	if (new_data == NULL || new_info == NULL)
		return (0);
	while (i < start)
	{
		new_data[j++] = line->data[i++];
		new_info[k++] = line->info[i++];
	}
	while (data && data[k])
	{
		new_data[j++] = data[k++];
		new_info[k++] = T_LITERAL;
	}
	while (line->data[end])
	{
		new_data[j++] = line->data[end++];
		new_info[j++] = line->info[end++];
	}
	new_data[j] = '\0';
	free(line->data);
	free(line->info);
	line->data = new_data;
	line->info = new_info;
	return (ft_strlen(data));
}
