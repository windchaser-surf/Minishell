/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 13:31:29 by rluari            #+#    #+#             */
/*   Updated: 2024/01/18 19:19:34 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int ft_strlen(char *str)
{
	int i = 0;

	while (str[i])
		i++;
	return (i);
}

char *ft_strchr(char *str, int c)
{
	int i = 0;
	
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

char	*ft_strjoin(char *str, char *buf)
{
	int i = -1, j = 0;
	int len;
	char *res;

	if (!str)
	{
		str = (char *)malloc(sizeof(char) * 1);
		if (!str)
			return (NULL);
		str[0] = '\0';
	}
	if (!str || !buf)
		return (NULL);
	len = ft_strlen(str) + ft_strlen(buf);
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
	int i = 0, j = 0;
	char *new;

	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
		return(free(str), NULL);
	new = (char *)malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	if (!new)	
		return (NULL);
	i++;
	while (str[i])
		new[j++] = str[i++];
	new[j] = '\0';
	free(str);
	return (new);
}

char	*ft_line(char *str)
{
	int i = 0;
	char *line;
	
	if (!str[i])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	line = (char *)malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		line[i] = str[i];
		i++;
	}	
	if (str[i] == '\n')
	{
		line[i] = str[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*ft_read(int fd, char *str)
{
	char 	*buf;
	int		rc = 1;

	buf = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	while (!ft_strchr(str, '\n') && rc != 0)
	{
		rc = read(fd, buf, BUFFER_SIZE);
		if (rc == -1)
			return(free(buf), free(str), NULL);
		buf[rc] = '\0';
		str = ft_strjoin(str, buf);
	}
	free(buf);
	return (str);
	
}

char *get_next_line(int fd)
{
	char *line;
	static char *str;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	str = ft_read(fd, str);
	if (!str)
		return (NULL);
	line = ft_line(str);
	str = ft_new(str);
	return (line);
}