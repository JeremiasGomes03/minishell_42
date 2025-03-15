/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_with_free.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:53:45 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/15 17:55:28 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strjoin_with_free(char *s1, char *s2, int free_s1)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	if (free_s1 && s1)
		free(s1);
	return (result);
}

