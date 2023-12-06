/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_do_the_comparison.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 13:37:43 by rluari            #+#    #+#             */
/*   Updated: 2023/11/06 19:29:31 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../libft.h"

static void	ft_do_normal_char_check(const char *input_str, int *i,
	int *char_counter, va_list *arg_l)
{
	if (input_str[*i] == 's')
		*char_counter += ft_print_string(arg_l);
	else if (input_str[*i] == 'c')
		*char_counter += ft_print_char(arg_l);
	else if (input_str[*i] == 'p')
		*char_counter += ft_print_pointer(arg_l);
	else if (input_str[*i] == 'd' || input_str[*i] == 'i')
		*char_counter += ft_print_dec(arg_l);
	else if (input_str[*i] == 'u')
		*char_counter += ft_print_uns(arg_l);
	else if (input_str[*i] == 'x' || input_str[*i] == 'X')
		*char_counter += ft_print_hex(arg_l, input_str[*i]);
	else if (input_str[*i] == '%')
	{
		ft_putchar_fd('%', 1);
		*char_counter += 1;
	}
}

void	ft_do_the_comparison(const char *input_str,
	va_list *arg_l, int *char_counter)
{
	int	i;

	i = 0;
	while (input_str[i])
	{
		if (input_str[i] == '%')
		{
			i++;
			ft_do_normal_char_check(input_str, &i, char_counter, arg_l);
		}
		else
		{
			write (1, &input_str[i], 1);
			(*char_counter)++;
		}
		i++;
	}
}
