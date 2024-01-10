/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_ctl_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 03:56:07 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:22:03 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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

void	replace_line_free(int i[], char *new_data, char *new_info, t_line *line)
{
	new_data[i[0]] = '\0';
	new_info[i[0]] = '\0';
	if (line->data)
		free(line->data);
	if (line->info)
		free(line->info);
	line->data = new_data;
	line->info = new_info;
}

int	replace_alloc(int *i, char *data[], t_line *new, t_line *line)
{
	if (new->data == NULL || new->info == NULL)
		return (1);
	i[0] = -1;
	i[1] = 0;
	data[0] = ft_malloc((ft_strlen(line->data) + ft_strlen(new->data) + 1));
	data[1] = ft_malloc((ft_strlen(line->info) + ft_strlen(new->info) + 1));
	return (0);
}

int	ft_delete_line(int len, t_line **line, int start, int flag)
{
	int		i;
	char	*new_data;
	char	*new_info;

	i = -1;
	if (ft_strlen((*line)->data) <= len && flag == 1)
		return (ft_delete_line_free(line));
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
	replace_line_free(&i, new_data, new_info, *line);
	return (len);
}

int	replace_line(t_line *origin, t_line **line, int start, int end)
{
	int		i[2];
	char	*data[2];

	if (replace_alloc(i, data, origin, *line))
		return (0);
	while (++i[0] < start)
	{
		data[0][i[0]] = (*line)->data[i[0]];
		data[1][i[0]] = (*line)->info[i[0]];
	}
	while (i[0] - start < ft_strlen(origin->data))
	{
		data[0][i[0]] = origin->data[i[1]];
		data[1][i[0]] = origin->info[i[1]];
		i[0]++;
		i[1]++;
	}
	while ((*line)->data[(++end - 1)])
	{
		data[0][i[0]] = (*line)->data[end - 1];
		data[1][i[0]++] = (*line)->info[end - 1];
	}
	replace_line_free(i, data[0], data[1], *line);
	free_line(origin);
	return (i[1]);
}
