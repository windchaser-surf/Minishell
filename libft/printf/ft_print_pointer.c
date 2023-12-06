/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:28:16 by rluari            #+#    #+#             */
/*   Updated: 2023/09/25 16:07:28 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_get_ptr_len(__uintmax_t ptr_value)
{
	int	i;

	i = 0;
	while (ptr_value != 0)
	{
		ptr_value /= 16;
		i++;
	}
	return (i);
}

int	ft_print_pointer(va_list *a_l)
{
	char		*ptr_array;
	int			len;
	__uintmax_t	ptr_value;
	int			i;

	ptr_value = va_arg(*a_l, __uintmax_t);
	if (!ptr_value)
		return (ft_printf("%s", "(nil)"));
	len = ft_get_ptr_len(ptr_value);
	i = len;
	ptr_array = (char *)malloc(len);
	if (!ptr_array)
		return (0);
	write(1, "0x", 2);
	while (--i >= 0)
	{
		if ((ptr_value % 16) < 10)
			ptr_array[i] = '0' + (ptr_value % 16);
		else
			ptr_array[i] = 'a' + (ptr_value % 16) - 10;
		ptr_value /= 16;
	}
	write(1, ptr_array, len);
	free(ptr_array);
	return (len + 2);
}
