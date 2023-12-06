/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_dec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:35:12 by rluari            #+#    #+#             */
/*   Updated: 2023/11/06 19:24:02 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_dec(va_list *a_l)
{
	int	num;
	int	len;

	num = va_arg(*a_l, int);
	len = ft_putnbr_fd_int(num, 1);
	return (len);
}
