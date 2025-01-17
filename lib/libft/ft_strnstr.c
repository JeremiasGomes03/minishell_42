/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 20:34:51 by jerda-si          #+#    #+#             */
/*   Updated: 2024/11/13 20:21:35 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t size)
{
	char	*start;
	char	*substr;
	size_t	i;

	i = 0;
	if (*to_find == '\0')
		return ((char *)(str));
	while (i < size && *str != '\0')
	{
		start = (char *)str;
		substr = (char *)to_find;
		while (*substr != '\0' && *str != '\0'
			&& *str == *substr && (i + (substr - to_find)) < size)
		{
			str++;
			substr++;
		}
		if (*substr == '\0')
			return (start);
		str = start + 1;
		i++;
	}
	return (NULL);
}
