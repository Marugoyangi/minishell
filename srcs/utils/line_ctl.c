/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_ctl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 03:56:07 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/16 14:50:32 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_line	*subline(t_line *line, int start, int end)
{
	t_line	*sub;
	int		i;

	i = 0;
	if (line == NULL)
		return (NULL);
	if (end - start == 0)
		return (NULL);
	sub = (t_line *)ft_malloc(sizeof(t_line));
	sub->data = (char *)ft_malloc(sizeof(char) * (end - start + 1));
	sub->info = (char *)ft_malloc(sizeof(char) * (end - start + 1));
	while (start < end)
	{
		sub->data[i] = line->data[start];
		sub->info[i] = line->info[start];
		i++;
		start++;
	}
	sub->data[i] = '\0';
	sub->info[i] = '\0';
	return (sub);
}

void	replace_line_free(int i, char *new_data, char *new_info, t_line *line)
{
	new_data[i] = '\0';
	new_info[i] = '\0';
	free(line->data);
	free(line->info);
	line->data = new_data;
	line->info = new_info;
}

int	replace_alloc(int *i, char *data[], t_line *new, t_line *line)
{
	if (new->data == NULL || new->info == NULL)
		return (1);
	*i = -1;
	data[0] = ft_malloc((ft_strlen(line->data) + ft_strlen(new->data) + 1));
	data[1] = ft_malloc((ft_strlen(line->info) + ft_strlen(new->info) + 1));
	return (0);
}

int	ft_delete_line(int len, t_line **line, int start)
{
	int		i;
	char	*new_data;
	char	*new_info;

	i = -1;
	if (len <= 0)
		return (0);
	new_data = (char *)ft_malloc(sizeof(char) * \
	(ft_strlen((*line)->data) - len + 1));
	new_info = (char *)ft_malloc(sizeof(char) * \
	(ft_strlen((*line)->info) - len + 1));
	while (++i < start - len)
	{
		new_data[i] = (*line)->data[i];
		new_info[i] = (*line)->info[i];
	}
	while ((*line)->data[start])
	{
		new_data[i] = (*line)->data[start];
		new_info[i] = (*line)->info[start];
		i++;
		start++;
	}
	replace_line_free(i, new_data, new_info, *line);
	return (len);
}

int	replace_line(t_line *origin, t_line **line, int start, int end)
{
	int		i;
	char	*data[2];

	if (replace_alloc(&i, data, origin, *line))
		return (0);
	while (++i < start)
	{
		data[0][i] = (*line)->data[i];
		data[1][i] = (*line)->info[i];
	}
	while (i - start < ft_strlen(origin->data))
	{
		data[0][i] = origin->data[i - start];
		data[1][i] = origin->info[i - start];
		i++;
	}
	while ((*line)->data[(++end - 1)])
	{
		data[0][i] = (*line)->data[end - 1];
		data[1][i] = (*line)->info[end - 1];
		i++;
	}
	replace_line_free(i, data[0], data[1], *line);
	free_line(origin);
	return (ft_strlen(origin->data) - (end - start));
}
