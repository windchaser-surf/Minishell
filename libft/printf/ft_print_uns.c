/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_uns.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:19:45 by rluari            #+#    #+#             */
/*   Updated: 2023/09/25 16:09:37 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_print_non_null_uns(unsigned int n, int fd)
{
	if (n >= 10)
		ft_print_non_null_uns(n / 10, fd);
	write(fd, &(char){n % 10 + '0'}, 1);
}

static int	ft_get_int_digits(unsigned int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static int	ft_putnbr_fd_uns(unsigned int n, int fd)
{
	int		len;

	len = ft_get_int_digits(n);
	if (n == 0)
	{
		write(fd, "0", 1);
		return (1);
	}
	ft_print_non_null_uns(n, fd);
	return (len);
}

int	ft_print_uns(va_list *a_l)
{
	unsigned int	num;
	int				len;

	num = va_arg(*a_l, unsigned int);
	len = ft_putnbr_fd_uns(num, 1);
	return (len);
}
