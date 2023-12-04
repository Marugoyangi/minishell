/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_ctl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 03:56:07 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/05 03:15:59 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	store_data(char *word, t_line *line, int i, char c)
{
	while (line->info[i] != c && line->info[i] != '\0')
	{
		word[i] = line->data[i];
		i++;
	}
	word[i] = '\0';
	return (1);
}

int	alloc_data(char **p, t_line *line, char c, int words)
{
	int	i;
	int	j;
	int	word;

	i = 0;
	word = 0;
	while (line->info[i])
	{
		while (line->info[i] == c)
			i++;
		j = 0;
		while (line->info[i + j] != c && line->info[i + j] != '\0')
			j++;
		if (words == word)
			return (-1);
		p[word] = (char *)malloc(sizeof (char) * (j + 1));
		word += store_data(p[word], line, i, c);
		i += j;
	}
	return (-1);
}

char	**line_to_data(t_line *line)
{
	int		words;
	char	**data;

	words =	ft_count_words(line->info, T_SPACE);
	data = (char **)malloc(sizeof(char *) * (words + 1));
	if (data == NULL)
		return (NULL);
	if (alloc_data(data, line, T_SPACE, words) == -1)
		return (NULL);
	data[words] = NULL;
	return (data);
}

int	ft_delete_line(int len, t_line *line, int start)
{
	int		i;
	char	*new_data;
	char	*new_info;

	i = -1;
	new_data = (char *)malloc(sizeof(char) * (ft_strlen(line->data) - len + 1));
	new_info = (char *)malloc(sizeof(char) * (ft_strlen(line->info) - len + 1));
	if (new_data == NULL || new_info == NULL)
		return (0);
	while (++i < start - len)
	{
		new_data[i] = line->data[i];
		new_info[i] = line->info[i];
	}
	while (line->data[start])
	{
		new_data[i] = line->data[start];
		new_info[i] = line->info[start];
		i++;
		start++;
	}
	new_data[i] = '\0';
	new_info[i] = '\0';
	free(line->data);
	free(line->info);
	line->data = new_data;
	line->info = new_info;
	printf("line->data : %s\n", line->data);
	printf("line->info : %s\n", line->info);
	return (len);
}

int	add_line(t_line *data, t_line *line, int start, int end)
{
	int		i;
	int		j;
	char	*new_data;
	char	*new_info;

	i = -1;
	j = 0;
	new_data = (char *)malloc(sizeof(char) * (ft_strlen(line->data) + \
	ft_strlen(data->data) + 1));
	new_info = (char *)malloc(sizeof(char) * (ft_strlen(line->info) + \
	ft_strlen(data->info) + 1));
	if (new_data == NULL || new_info == NULL)
		return (0);
	while (++i < start)
	{
		new_data[i] = line->data[i];
		new_info[i] = line->info[i];
	}
	while (data->info[j])
	{
		new_data[i] = data->data[j];
		new_info[i] = data->info[j];
		i++;
		j++;
	}
	while (line->data[end])
	{
		new_data[i] = line->data[end];
		new_info[i] = line->info[end];
		i++;
		end++;
	}
	new_data[i] = '\0';
	new_info[i] = '\0';
	free(line->data);
	free(line->info);
	line->data = new_data;
	line->info = new_info;
	printf("ft_strlen(data->data) : %d\n", ft_strlen(data->data));
	return (ft_strlen(data->info));
}

int	replace_line(char *data, t_line *line, int start, int end) //길이 줄이거나 나누기
{
	int		i;
	int		j;
	char	*new_data;
	char	*new_info;

	i = -1;
	j = 0;
	new_data = (char *)malloc(sizeof(char) * (ft_strlen(line->data) + \
	ft_strlen(data) + 1));
	new_info = (char *)malloc(sizeof(char) * (ft_strlen(line->info) + \
	ft_strlen(data) + 1));
	if (new_data == NULL || new_info == NULL)
		return (0);
	while (++i < start)
	{
		new_data[i] = line->data[i];
		new_info[i] = line->info[i];
	}
	while (i - start < ft_strlen(data))
	{
		new_data[i] = data[j++];
		new_info[i++] = T_LITERAL;
	}
	while (line->data[end])
	{
		new_data[i] = line->data[end];
		new_info[i] = line->info[end];
		i++;
		end++;
	}
	new_data[i] = '\0';
	new_info[i] = '\0';
	free(line->data);
	free(line->info);
	line->data = new_data;
	line->info = new_info;
	return (ft_strlen(data));
}
