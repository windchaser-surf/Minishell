/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 09:37:22 by rluari            #+#    #+#             */
/*   Updated: 2023/11/28 15:25:42 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_get_int_digits(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n == INT_MIN)
		return (11);
	if (n < 0)
	{
		i++;
		n *= -1;
	}
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		end;

	end = ft_get_int_digits(n);
	res = (char *)malloc((sizeof(char)) * (end + 1));
	if (!res)
		return (NULL);
	if (n == 0)
		return (ft_strcpy(res, "0"));
	if (n == INT_MIN)
		return (ft_strcpy(res, "-2147483648"));
	res[ft_get_int_digits(n)] = '\0';
	if (n < 0)
	{
		res[0] = '-';
		n *= -1;
	}
	while (n > 0)
	{
		res[end-- - 1] = (char)(n % 10 + '0');
		n /= 10;
	}
	return (res);
}
