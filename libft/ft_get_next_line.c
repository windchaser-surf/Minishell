/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 13:31:29 by rluari            #+#    #+#             */
/*   Updated: 2023/11/28 15:23:44 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_get_next_line.h"
#include "libft.h"
#include <stdlib.h>

struct s_tmp {
	int		i;
	int		len;
	char	*res;
};

char	*update_leftover_str(char *leftover_str)
{
	char	*new_lftov;
	int		j;
	int		i;

	i = 0;
	j = 0;
	while (leftover_str[i] != '\n' && leftover_str[i])
		i++;
	new_lftov = (char *)malloc((int)ft_strlen(leftover_str) - i + 1);
	if (!new_lftov)
		return (NULL);
	if (leftover_str[i])
		i++;
	while (leftover_str[i] != '\0')
		new_lftov[j++] = leftover_str[i++];
	new_lftov[j] = '\0';
	if (j == 0)
	{
		free(new_lftov);
		new_lftov = NULL;
	}
	free(leftover_str);
	return (new_lftov);
}

char	*ft_make_str(char *leftover_str)
{
	struct s_tmp	vars;

	vars.i = -1;
	vars.len = 0;
	while (leftover_str[vars.len] && leftover_str[vars.len] != '\n')
		vars.len++;
	if (leftover_str[vars.len] == '\n')
		vars.res = (char *)malloc(vars.len + 2);
	else
		vars.res = (char *)malloc(vars.len + 1);
	if (!vars.res)
		return (NULL);
	while (++vars.i < vars.len)
		vars.res[vars.i] = leftover_str[vars.i];
	if (leftover_str[vars.i] == '\n')
	{
		vars.res[vars.i] = leftover_str[vars.i];
		vars.i++;
	}
	vars.res[vars.i] = '\0';
	return (vars.res);
}

char	*read_stuff(char *leftover_str, int fd)
{
	int			chnum;
	char		*line;

	line = (char *)malloc(BUFFER_SIZE + 1);
	if (!line)
		return (NULL);
	chnum = 1;
	line[0] = '\0';
	while (chnum > 0 && (!ft_strchr(line, '\n')))
	{
		chnum = read(fd, line, BUFFER_SIZE);
		if (chnum < 0)
		{
			free (line);
			if (leftover_str)
				free(leftover_str);
			return (NULL);
		}
		line[chnum] = '\0';
		leftover_str = ft_strjoin_gnl(leftover_str, line);
	}
	free(line);
	return (leftover_str);
}

char	*get_next_line(int fd)
{
	char		*res;
	static char	*leftover_str;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	leftover_str = read_stuff(leftover_str, fd);
	if (!leftover_str)
		return (NULL);
	res = ft_make_str(leftover_str);
	if (!res)
	{
		free(leftover_str);
		return (NULL);
	}
	leftover_str = update_leftover_str(leftover_str);
	return (res);
}
/*
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *str;
    char    *path;
    int     i;

    path = "source.txt";
    fd = open(path, O_RDONLY);
    i = 0;
    while (i < 6)
    {
        str = (get_next_line(fd));
        printf("fd %i: %s\n", i, str);
        free(str);
        i++;
    }
	close(fd);
    return (0);
}*/
