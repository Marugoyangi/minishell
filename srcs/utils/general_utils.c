/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 03:31:45 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/15 03:31:45 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*modified_strjoin(char *s1, char *s2, int flag)
{
	char	*str;
	int		i;
	int		k;

	str = ft_malloc(sizeof (char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	i = -1;
	while (s1 && s1[++i])
		str[i] = s1[i];
	k = 0;
	while (s2 && s2[k])
	{
		str[i + k] = s2[k];
		k++;
	}
	str[i + k] = 0;
	if (flag == 1)
		free(s1);
	else if (flag == 2)
		free(s2);
	else if (flag == 0)
	{
		free(s1);
		free(s2);
	}
	return (str);
}

char	*modified_substr(char *s, int start, int len)
{
	char	*str;
	int		i;

	if (s == 0)
		return (0);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	str = (char *)ft_malloc(sizeof (char) * (len + 1));
	i = 0;
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = 0;
	free(s);
	return (str);
}

char	*modified_strtrim(char *s1, char const *set)
{
	int		start;
	int		end;
	char	*str;

	if (set == 0)
		return ((char *)s1);
	start = -1;
	end = ft_strlen(s1) + 1;
	while (s1 && s1[++start])
		if (!ft_strchr(set, s1[start]))
			break ;
	while (--end > 0)
		if (!ft_strchr(set, s1[end - 1]))
			break ;
	if (start > end)
	{
		free(s1);
		return (NULL);
	}
	str = ft_malloc(end - start + 1);
	ft_strlcpy(str, s1 + start, end - start + 1);
	free(s1);
	return (str);
}

char	*modified_strdup(char *s)
{
	char	*new_s;
	int		i;

	i = 0;
	if (s == 0)
		return (0);
	new_s = (char *)ft_malloc(sizeof(char) * (ft_strlen(s) + 1));
	while (s[i])
	{
		new_s[i] = s[i];
		i++;
	}
	new_s[i] = s[i];
	free(s);
	return (new_s);
}

void	*ft_malloc(int size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr == NULL)
	{
		perror("minishell");
		exit(E_MALLOC);
	}
	return (ptr);
}
