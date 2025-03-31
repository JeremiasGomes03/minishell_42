/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:22:13 by jerda-si          #+#    #+#             */
/*   Updated: 2025/03/31 00:37:53 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	size_t		len;
	char		*dup;

	len = ft_strlen(str) + 1;
	dup = malloc(len);
	if (!dup)
	{
		perror("malloc");
		exit(1);
	}
	ft_memcpy(dup, str, len);
	return (dup);
}
