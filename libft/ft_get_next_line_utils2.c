/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 21:04:50 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 21:05:24 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	ft_strlen2(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strchr2(char *str, int c)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == (char)c)
			return ((char *)(str + i));
		i++;
	}
	if (c == '\0')
		return ((char *)(str + i));
	return (NULL);
}

char	*ft_helper_strhoin(char *str)
{
	if (!str)
	{
		str = (char *)malloc(sizeof(char) * 1);
		if (!str)
			return (NULL);
		str[0] = '\0';
	}
	return (str);
}

char	*ft_strjoin2(char *str, char *buf)
{
	int		i;
	int		j;
	int		len;
	char	*res;

	i = -1;
	j = 0;
	str = ft_helper_strhoin(str);
	if (!str || !buf)
		return (NULL);
	len = ft_strlen2(str) + ft_strlen2(buf);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	if (str)
		while (str[++i])
			res[i] = str[i];
	while (buf[j])
		res[i++] = buf[j++];
	res[len] = '\0';
	free(str);
	return (res);
}

char	*ft_new(char *str)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
		return (free(str), NULL);
	new = (char *)malloc(sizeof(char) * (ft_strlen2(str) - i + 1));
	if (!new)
		return (NULL);
	i++;
	while (str[i])
		new[j++] = str[i++];
	new[j] = '\0';
	free(str);
	return (new);
}
