/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:37:41 by rluari            #+#    #+#             */
/*   Updated: 2023/11/07 11:58:25 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../libft.h"

static void	ft_make_hex(unsigned int hex, int *chars, char c, int fd)
{
	if (hex >= 16)
	{
		ft_make_hex(hex / 16, chars, c, fd);
		ft_make_hex(hex % 16, chars, c, fd);
	}
	else
	{
		if (hex <= 9)
			ft_putchar_fd((hex + '0'), fd);
		else
		{
			if (c == 'x')
				ft_putchar_fd((hex - 10 + 'a'), fd);
			if (c == 'X')
				ft_putchar_fd((hex - 10 + 'A'), fd);
		}
		*chars += 1;
	}
}

int	ft_print_hex(va_list *a_l, char c)
{
	unsigned int	hex;
	int				chars;
	int				fd;

	fd = 1;
	hex = va_arg(*a_l, unsigned int);
	chars = 0;
	if (hex == 0)
	{
		ft_putchar_fd('0', fd);
		return (1);
	}
	ft_make_hex(hex, &chars, c, fd);
	return (chars);
}
