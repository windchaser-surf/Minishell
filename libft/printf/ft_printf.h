/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:55:13 by rluari            #+#    #+#             */
/*   Updated: 2023/11/06 18:53:02 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

int		ft_printf(const char *input_str, ...);
int		ft_print_string(va_list *a_l);
int		ft_print_char(va_list *a_l);
int		ft_print_pointer(va_list *a_l);
int		ft_print_dec(va_list *a_l);
int		ft_print_uns(va_list *a_l);
int		ft_print_hex(va_list *a_l, char c);
void	ft_do_the_comparison(const char *str, va_list *arg_l,
			int *char_counter);
int		ft_putnbr_fd_int(int n, int fd);
//int		ft_putchar_fd(char c, int fd);

#endif