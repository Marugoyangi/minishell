/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asterisk_filter_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:25:31 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:25:31 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	filtered_node(struct dirent *file, t_node **result, char *pwd, \
						char **line)
{
	t_node	*tmp;
	char	*tmp_pwd;

	tmp_pwd = NULL;
	if (line[1][0] != '\0' && file->d_type == DT_REG)
	{
		free(pwd);
		return ;
	}
	tmp = last_node(*result);
	tmp_pwd = modified_strjoin(pwd, ft_strjoin("/", file->d_name), 0);
	if (line[1][0] != '\0')
		tmp_pwd = modified_strjoin(tmp_pwd, "/", 1);
	if (tmp == NULL)
		*result = create_node(ft_strdup(tmp_pwd), NULL, L_SIMPLE_COMMAND);
	else
		tmp->right = create_node(ft_strdup(tmp_pwd), NULL, L_SIMPLE_COMMAND);
	free(tmp_pwd);
}

void	asterisk_recursive(t_node **result, char **line, char *name, int *depth)
{
	char	*pwd;
	char	*filename;
	int		i;

	(*depth)++;
	if (*depth == 2)
	{
		i = ft_strlen(name);
		while (i >= 0 && name[i] != '/')
			i--;
		pwd = modified_strjoin(ft_substr(name, 0, i), "/", 1);
		filename = modified_substr(name, i + 1, ft_strlen(name) - i - 1);
		pwd = modified_strjoin(pwd, filename, 2);
		asterisk_subdir(result, line, pwd, depth);
	}
	else
		asterisk_subdir(result, line, name, depth);
	(*depth)--;
}

int	is_file_or_dir(char **line, int *depth, int is_hidden, struct dirent *file)
{
	if (file == NULL)
		return (-1);
	if (!check_asterisk(line, file->d_name, depth))
		return (-2);
	if (line[*depth + 1] == NULL && ((is_hidden == 0 && file->d_name[0] != '.') \
	|| is_hidden == 1))
		return (1);
	else if (file->d_type == DT_DIR && line[*depth + 1] != NULL && \
	((is_hidden == 0 && file->d_name[0] != '.') || is_hidden == 1))
		return (2);
	return (0);
}

void	asterisk_subdir(t_node **result, char **line, char *pwd, int *depth)
{
	DIR				*dir;
	struct dirent	*file;
	int				is_hidden;

	dir = opendir(pwd);
	is_hidden = asterisk_exceptions(dir, line, depth);
	if (is_hidden == -1 || is_hidden == -2)
		return (free(pwd));
	while (1)
	{
		file = readdir(dir);
		if (is_file_or_dir(line, depth, is_hidden, file) == -1)
			break ;
		else if (is_file_or_dir(line, depth, is_hidden, file) == -2)
			continue ;
		else if (is_file_or_dir(line, depth, is_hidden, file) == 1)
			filtered_node(file, result, ft_strdup(pwd), line);
		else if (is_file_or_dir(line, depth, is_hidden, file) == 2)
			asterisk_recursive(result, line, modified_strjoin(pwd, \
			ft_strjoin("/", file->d_name), 2), depth);
	}
	free(pwd);
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
	result = (t_node **)ft_malloc(sizeof(t_node *));
	*result = NULL;
	if (line[0] == NULL)
	{
		line[0] = getcwd(NULL, 0);
		is_current_pwd = 1;
	}
	asterisk_subdir(result, line, ft_strdup(line[0]), &depth);
	tmp = *result;
	while (tmp)
	{
		tmp->data = modified_substr(tmp->data, ft_strlen(line[0]) + 1, \
		ft_strlen(tmp->data) - ft_strlen(line[0]) - 1);
		if (is_current_pwd != 1)
			tmp->data = modified_strjoin(line[0], tmp->data, 2);
		tmp = tmp->right;
	}
	return (result);
}
