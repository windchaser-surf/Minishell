/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 13:36:38 by rluari            #+#    #+#             */
/*   Updated: 2023/09/25 16:07:30 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_string(va_list *a_l)
{
	char	*string;		
	int		chars;

	chars = 0;
	string = va_arg(*a_l, char *);
	if (!string)
		return (ft_printf("%s", "(null)"));
	while (*string)
	{
		write (1, string, 1);
		chars++;
		string++;
	}
	return (chars);
}
