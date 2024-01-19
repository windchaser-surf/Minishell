/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 13:34:37 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 21:07:17 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GET_NEXT_LINE_H
# define FT_GET_NEXT_LINE_H
# define BUFFER_SIZE 4096

# include <unistd.h>
# include <stdlib.h>

char	*get_next_line(int fd);
char	*ft_strjoin_gnl(char *leftov_str, char *line);

int		ft_strlen2(char *str);
char	*ft_strchr2(char *str, int c);
char	*ft_strjoin2(char *str, char *buf);
char	*ft_new(char *str);
char	*ft_helper_strhoin(char *str);

#endif