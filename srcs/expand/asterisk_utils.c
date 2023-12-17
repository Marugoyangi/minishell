/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asterisk_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 15:07:20 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/16 15:18:21 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_asterisk_iter(int *i, int *depth, char **filter, char *file)
{
	if (filter[*depth][i[0]] == -T_ASTERISK)
	{
		while (filter[*depth][i[0]] == -T_ASTERISK)
			i[0]++;
		if (filter[*depth][i[0]] == '\0')
			return (1);
		i[1]++;
		while (file[i[1]] && file[i[1]] != filter[*depth][i[0]])
			i[1]++;
		if (file[i[1]] == '\0')
			return (0);
	}
	else if (filter[*depth][i[0]] == file[i[1]])
	{
		i[0]++;
		i[1]++;
	}
	else
		return (-1);
	return (-2);
}

int	check_asterisk(char **filter, char *file, int *depth)
{
	int	i[2];
	int	res;

	i[0] = 0;
	i[1] = 0;
	res = 0;
	while (filter[*depth][i[0]] || file[i[1]])
	{
		res = check_asterisk_iter(i, depth, filter, file);
		if (res == -1)
			break ;
		else if (res == 1)
			return (1);
		else if (res == 0)
			return (0);
		else if (res == -2)
			continue ;
	}
	if (filter[*depth][i[0]] == '\0' && file[i[1]] == '\0')
		return (1);
	return (0);
}

int	asterisk_exceptions(DIR *dir, char **line, int *depth)
{
	if (dir == NULL)
		return (-2);
	if (line[*depth][0] != '.')
		return (0);
	else if (line[*depth][0] == -T_ASTERISK && line[*depth][1] == '.')
		return (-1);
	else
		return (1);
}
