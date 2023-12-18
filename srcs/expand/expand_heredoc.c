/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 11:06:17 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/19 02:16:34 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_heredoc_var(t_line **line, t_arg *arg, int *i, int *start)
{
	t_line	*env_line;
	char	*env;
	char	*tmp;

	while ((*line)->info[*i] && (is_not_delimiter((*line)->data[*i]) || \
		is_number((*line)->data[*i])))
		(*i)++;
	if (*start != -1)
	{
		tmp = ft_substr((*line)->data, *start + 1, *i - *start - 1);
		if (!ft_strcmp(tmp, "?"))
			env = ft_itoa(arg->last_exit_status);
		else
			env = ft_strdup(find_env(arg->envp_head, tmp));
		free(tmp);
		if (!env)
			return ;
		else
		{
			env_line = get_info(env, 0);
			*i += replace_line(env_line, line, *start, *i) - (*i - *start);
		}
	}
}

void	expand_heredoc_env(t_line **line, t_arg *arg)
{
	int		i;
	int		start;

	i = 0;
	while ((*line) && (*line)->info && (*line)->info[i])
	{
		while ((*line)->info[i] && (*line)->info[i] != T_ENV)
			i++;
		while ((*line)->info[i] && ((*line)->info[i] == T_ENV))
			i++;
		if (i > 0 && (*line)->info[i - 1] == T_ENV && (*line)->info[i])
			start = i - 1;
		if (i > 0 && (*line)->info[i - 1] == T_ENV && \
		!is_number((*line)->data[i]) \
			&& (*line)->data[i] != '\0')
			i++;
		else
			start = -1;
		expand_heredoc_var(line, arg, &i, &start);
		start = -1;
	}
}

void	expand_heredoc_str(t_node *node, char *str, t_arg *arg, int fd)
{
	t_line	*line;
	int		i;

	i = 0;
	line = (t_line *)ft_malloc(sizeof(t_line));
	line->data = str;
	line->info = (char *)ft_malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (line->data[i])
	{
		if (line->data[i] == '$')
			line->info[i] = T_ENV;
		else
			line->info[i] = T_WORD;
		i++;
	}
	line->info[i] = '\0';
	expand_heredoc_env(&line, arg);
	close (fd);
	open (node->filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	write(fd, line->data, ft_strlen(line->data));
	free(line->data);
	free(line->info);
	free(line);
}

void	read_heredoc_file(int fd, char **str, char *buf)
{
	int		byte;

	*str = ft_strdup("");
	while (1)
	{
		byte = read(fd, buf, 2048);
		if (byte < 0)
		{
			perror("minishell");
			break ;
		}
		else if (byte == 0)
			break ;
		else
		{
			buf[byte] = '\0';
			*str = modified_strjoin(*str, buf, 1);
		}
	}
}

void	expand_heredoc(t_arg *arg)
{
	t_node	*tmp;
	int		fd;
	char	buf[2049];
	char	*str;

	tmp = arg->ast_head;
	str = NULL;
	while (tmp)
	{
		if (tmp->data && tmp->type == L_REDIRECTION && \
		!ft_strcmp(tmp->data, "<<") && tmp->filename && \
		tmp->argv && tmp->argv[0])
		{
			if (tmp->filename)
				fd = open(tmp->filename, O_RDWR, 0644);
			if (fd < 0)
				perror("minishell");
			else
				read_heredoc_file(fd, &str, buf);
			expand_heredoc_str(tmp, str, arg, fd);
			close(fd);
		}
		tmp = tmp->right;
	}
}
