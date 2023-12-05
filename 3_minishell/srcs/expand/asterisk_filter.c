/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asterisk.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 05:51:41 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/02 05:51:41 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	asterisk_exceptions(char *filter)
{
	int	i;

	i = 0;
	if (filter[0] != '.')
		return (0);
	else if (filter[0] == -T_ASTERISK && filter[1] == '.')
		return (-1);
	else
		return (1);
}

int	check_asterisk(char **filter, char *file, int *depth) // line은 규칙, file은 파일 또는 디렉토리
{
	int	i;
	int	j;

	i = 0; // filter의 인덱스
	j = 0; // file의 인덱스
	while (filter[*depth][i] || file[j])
	{
		if (filter[*depth][i] == -T_ASTERISK)
		{
			while (filter[*depth][i] == -T_ASTERISK)
				i++;
			if (filter[*depth][i] == '\0')
				return (1);
			j++;
			while (file[j] && file[j] != filter[*depth][i])
				j++;
			if (file[j] == '\0')
				return (0);
		}
		else if (filter[*depth][i] == file[j])
		{
			i++;
			j++;
		}
		else
			break ;
	}
	if (filter[*depth][i] == '\0' && file[j] == '\0')
		return (1);
	return (0);
}

void	filtered_node(struct dirent *file, t_node **result, char *pwd, \
						char *last_filter)
{
	t_node	*tmp;
	char	*tmp_pwd;

	if (last_filter && file->d_type == DT_REG)
		return ;
	tmp = last_node(*result);
	tmp_pwd = ft_strjoin(pwd, ft_strjoin("/", file->d_name));
	if (last_filter)
		tmp_pwd = ft_strjoin(tmp_pwd, "/");
	if (tmp == NULL)
		*result = create_node(tmp_pwd, 0, NULL);
	else
		tmp->right = create_node(tmp_pwd, 0, NULL);
}

void	asterisk_subdir(t_node **result, char **line, char *pwd, int *depth)
{
	DIR				*dir;
	struct dirent	*file;
	int				is_hidden;

	dir = opendir(pwd);
	if (dir == NULL)
		return ;
	is_hidden = asterisk_exceptions(line[*depth]);
	if (is_hidden == -1)
		return ;
	while (1)
	{
		file = readdir(dir); // 하나씩 읽어감
		if (file == NULL)
			break ;
		if (!check_asterisk(line, file->d_name, depth))
			continue ;
		if (file->d_type == DT_DIR && line[*depth + 1] != NULL)
		{
			(*depth)++;
			if (*depth == 2)
				asterisk_subdir(result, line, ft_strjoin(pwd, file->d_name), depth);
			else
				asterisk_subdir(result, line, ft_strjoin(pwd, ft_strjoin("/", file->d_name)), depth);
			(*depth)--;
		}
		else if (line[*depth + 1] == NULL && is_hidden == 0 && file->d_name[0] != '.')
			filtered_node(file, result, pwd, line[1]);
		else if (line[*depth + 1] == NULL && is_hidden == 1 && file->d_name[0] == '.')
			filtered_node(file, result, pwd, line[1]);
	}
	closedir(dir);
}

t_node	**filter_asterisk(char **line)
{
	int		depth;
	t_node	**result;
	int		is_current_pwd;
	t_node	*tmp;

	depth = 2;
	is_current_pwd = 0;
	result = (t_node **)malloc(sizeof(t_node *));
	*result = NULL;
	if (line[0] == NULL)
	{
		line[0] = getcwd(NULL, 0);
		is_current_pwd = 1;
	}
	printf("pwd %s \n", line[0]);
	asterisk_subdir(result, line, line[0], &depth);
	tmp = *result;
	while (tmp)
	{
		if (is_current_pwd == 1)
		tmp->data = ft_substr(tmp->data, ft_strlen(line[0]) + 1, \
		ft_strlen(tmp->data) - ft_strlen(line[0]) - 1);
		tmp = tmp->right;
	}
	printf	("\n");
	return (result);
}
