#include "minishell.h"

char	*ft_substr(char const *s, int start, int len)
{
	char	*str;
	int		i;

	if (s == 0)
		return (0);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	str = (char *)malloc(sizeof (char) * (len + 1));
	if (str == 0)
		return (0);
	i = 0;
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = 0;
	return (str);
}
int ft_strlcpy(char *dest, const char *src, int size)
{
	int	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (i < size - 1 && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (ft_strlen(src));
}


int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}


char	*ft_malloc(size_t start, size_t end)
{
	char	*str;

	if (end <= start)
	{
		str = (char *)malloc(sizeof (char) * 1);
		if (str == 0)
			return (0);
		str[0] = '\0';
		return (str);
	}
	str = (char *)malloc(sizeof (char) * (end - start + 1));
	if (str == 0)
		return (0);
	return (str);
}

char *ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != (char)c && s[i])
		i++;
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*str;

	if (set == 0)
		return (ft_strdup(s1));
	start = 0;
	end = ft_strlen(s1);
	while (s1[start])
	{
		if (!ft_strchr(set, s1[start]))
			break ;
		start++;
	}
	while (end > 0)
	{
		if (!ft_strchr(set, s1[end - 1]))
			break ;
		end--;
	}
	str = ft_malloc(start, end);
	if (str != 0)
		ft_strlcpy(str, s1 + start, end - start + 1);
	free((char *)s1);
	return (str);
}




char	*ft_strdup(const char *s)
{
	char	*new_s;
	int		i;

	i = 0;
	new_s = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (new_s == NULL)
		return (NULL);
	while (s[i])
	{
		new_s[i] = s[i];
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}
