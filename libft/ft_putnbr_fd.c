/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 14:34:07 by rluari            #+#    #+#             */
/*   Updated: 2023/09/10 16:25:45 by rluari           ###   ########.fr       */
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

void	ft_putnbr_fd(int n, int fd)
{
	char	res[12];
	int		len;
	long	number;

	len = ft_get_int_digits(n);
	number = (long)n;
	if (n < 0)
	{
		res[0] = '-';
		number *= -1;
	}
	if (number == 0)
	{
		write(fd, "0", 1);
		return ;
	}
	while (number > 0)
	{
		res[len - 1] = (char)(number % 10 + '0');
		number /= 10;
		len--;
	}
	write(fd, res, ft_get_int_digits(n));
}
