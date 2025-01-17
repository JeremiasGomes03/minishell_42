/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 22:17:12 by jerda-si          #+#    #+#             */
/*   Updated: 2024/10/30 16:26:03 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_d(int nb)
{
	int	i;

	i = 0;
	if (nb < 0)
		i++;
	if (nb == 0)
		return (1);
	while (nb != 0)
	{
		nb /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char				*number;
	int					length;

	length = count_d(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	number = (char *)malloc(length + 1);
	if (!number)
		return (NULL);
	number[length] = '\0';
	if (n < 0)
	{
		number[0] = '-';
		n = -n;
	}
	if (n == 0)
		number[0] = '0';
	while (n)
	{
		number[--length] = (n % 10) + '0';
		n /= 10;
	}
	return (number);
}
