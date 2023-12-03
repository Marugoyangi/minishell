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

int	is_ending_slash(char *line) // line은 규칙
{
	int	i;

	i = 0;
	while (line[i])
		i++;
	if (line[i - 1] == '/')
		return (1);
	return (0);
}

int	check_asterisk(char *filter, char *file) // line은 규칙, file은 파일 또는 디렉토리
{
	int	i;
	int	j;

	i = 0; // filter의 인덱스
	j = 0; // file의 인덱스
	while (filter[i] && file[j])
	{
		if (filter[i] == -T_ASTERISK) // *이 나오면
		{
			while (filter[i] == -T_ASTERISK) // *이 끝날때까지
				i++;
			while (file[j] && file[j] != filter[i]) // *이 끝난 다음부터 일치할때까지
				j++;
		}
		else if (filter[i] == file[j]) // 일치하면
		{
			i++;
			j++;
		}
		else
			j++;
	}
	if (filter[i] == '\0') // 둘다 끝나면
		return (1);
	return (0);
}

int	recursive_finder(struct dirent *file, t_node *result, char *pwd, char **line)
{
	t_node			*tmp;
	int				depth;

	depth = 0;
	if (!check_asterisk(line[depth], file->d_name))
		return (0);
	if (file->d_type == DT_DIR)
	{
		depth++;
		asterisk_subdir(result, line, \
		ft_strjoin(pwd, ft_strjoin("/", file->d_name)), depth);
		depth--;
	}
	else if (file->d_type == DT_REG && line[depth + 1] == NULL)
	{
		tmp = last_node(result);
		if (tmp == NULL)
			result = create_node(ft_strjoin(line[0], \
			ft_strjoin("/", file->d_name)), 0, NULL);
		else
			tmp->right = create_node(ft_strjoin(line[0], \
			ft_strjoin("/", file->d_name)), 0, NULL);
	}
	return (depth);
}


void	asterisk_subdir(t_node *result, char **line, char *pwd, int depth)
{
	DIR				*dir;
	struct dirent	*file;

	dir = opendir(line[0]);
	if (dir == NULL)
		return ;
	while (1)
	{
		file = readdir(dir); // 하나씩 읽어감
		if (file == NULL)
			break ;
		depth = recursive_finder(file, result, pwd, line);
		if (depth == 0)
			continue ;
	}
	closedir(dir);
}

t_node	*filter_asterisk(char **line)
{
	int		depth;
	t_node	*result;
	t_node	*tmp;
	char	*pwd;

	depth = 0;
	result = NULL;
	pwd = getcwd(NULL, 0);
	if (line[0] == NULL)
		line[0] = ft_strdup(pwd);
	asterisk_subdir(result, line, pwd, depth);
	tmp = result;
	while ((tmp))
	{
		tmp->data = ft_substr(tmp->data, ft_strlen(line[0]) + 1, \
			ft_strlen(tmp->data) - ft_strlen(line[0]) - 1);
		printf("final->data : %s\n", tmp->data);
		tmp = tmp->right;
	}
	return (result);
}
