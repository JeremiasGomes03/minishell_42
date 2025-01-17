/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:02:49 by jerda-si          #+#    #+#             */
/*   Updated: 2024/11/01 18:33:34 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *s)
{
	int	neg;
	int	val;

	val = 0;
	neg = 1;
	while ((*s >= 9 && *s <= 13) || *s == ' ')
		s++;
	if (ft_isalpha(*s))
		return (0);
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			neg = -neg;
		s++;
	}
	while (ft_isdigit(*s))
	{
		val = val * 10 + (*s - '0');
		s++;
	}
	return (neg * val);
}
