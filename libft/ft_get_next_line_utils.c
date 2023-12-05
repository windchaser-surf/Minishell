/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 15:10:40 by rluari            #+#    #+#             */
/*   Updated: 2023/11/28 15:33:19 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_get_next_line.h"
#include "libft.h"

struct s_joinvars {
	int		full_len;
	int		i;
	int		j;
	char	*res;
};

char	*ft_strjoin_gnl(char *leftov_str, char const *line)
{
	struct s_joinvars	jv;

	jv.i = -1;
	jv.j = 0;
	if (!leftov_str)
	{
		leftov_str = (char *)malloc(1);
		leftov_str[0] = '\0';
	}
	jv.full_len = ft_strlen(leftov_str) + ft_strlen(line);
	if (jv.full_len == 0)
	{
		free(leftov_str);
		return (NULL);
	}
	jv.res = (char *)malloc(jv.full_len + 1);
	if (!jv.res)
		return (NULL);
	while (++jv.i < jv.full_len - (int)ft_strlen(line))
		jv.res[jv.i] = leftov_str[jv.i];
	while (jv.i < jv.full_len)
		jv.res[jv.i++] = line[jv.j++];
	jv.res[jv.i] = '\0';
	free(leftov_str);
	return (jv.res);
}

/*char	*ft_strchr(char *s, int c)
{
	int	i;

	i = 0;
	if ((char)c == '\0')
		return ((char *)s + ft_strlen(s));
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((s + i));
		i++;
	}
	return (NULL);

	while (*s != '\0')
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}*/

/*size_t	ft_strlen(const char *s)
{
	unsigned int	len;

	len = 0;
	while (s[len] != '\0')
	{
		len++;
	}
	return (len);
}*/