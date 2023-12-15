/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_libft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 18:54:11 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/15 09:29:00 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_perror(char *str);
int	ft_atoi(const char *nptr);
int	ft_strncmp(const char *s1, const char *s2, size_t n);

char **make_envp(t_env *envp_head)
{
	int		i;
	char	**envp;
	t_env	*cur;

	i = 0;
	cur = envp_head;
	while (cur)
	{
		cur = cur->next;
		i++;
	}
	envp = (char **)ft_malloc(sizeof(char *) * (i + 1));
	i = 0;
	cur = envp_head;
	while (cur)
	{
		envp[i] = ft_strjoin(cur->key, "=");
		envp[i] = ft_strjoin(envp[i], cur->value);
		cur = cur->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

int	exec_perror(char *str)
{
	perror(str);
	if (strcmp(str, "execve"))
		exit(1);
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int			i;
	int			sign;
	long long	res;

	i = 0;
	sign = 1;
	res = 0;
	while ((9 <= nptr[i] && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while ('0' <= nptr[i] && nptr[i] <= '9')
	{
		res = res * 10 + (nptr[i] - '0');
		i++;
	}
	return (sign * res);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (s1[i] == s2[i] && i + 1 < n && s1[i] != 0 && s2[i] != 0)
		++i;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
