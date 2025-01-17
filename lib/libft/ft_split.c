/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 22:18:21 by jerda-si          #+#    #+#             */
/*   Updated: 2024/10/31 12:25:19 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_c(char const *s, char c)
{
	size_t	count;

	count = 0;
	if (!*s)
		return (0);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

static void	free_split(char **result, int count)
{
	if (result)
	{
		while (count > 0)
			free(result[--count]);
		free(result);
	}
}

static size_t	count_len(char const *s, int c)
{
	size_t	len;

	len = 0;
	if (!ft_strchr((char *)s, c))
		len = ft_strlen(s);
	else
		len = ft_strchr((char *)s, c) - s;
	return (len);
}

char	**ft_split(char const *s, char c)
{
	size_t	len;
	char	**res;
	int		i;

	res = (char **)malloc((count_c(s, c) + 1) * sizeof(char *));
	if (!res || !s)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			len = count_len(s, c);
			res[i] = ft_substr(s, 0, len);
			if (!res[i])
				return (free_split(res, i), NULL);
			s += len;
			i++;
		}
	}
	res[i] = NULL;
	return (res);
}
