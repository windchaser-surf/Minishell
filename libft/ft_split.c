/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 11:47:15 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 20:54:56 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(const char *str, char target)
{
	int	count;

	count = 0;
	if (!str)
		return (0);
	while (*str == target)
		str++;
	if (*str != '\0' && *str != target)
		count++;
	else if (*str == '\0')
		return (0);
	while (*str != '\0')
	{
		if (*str == target)
		{
			while (*str == target)
				str++;
			if (*str == '\0')
				break ;
			count++;
			str--;
		}
		str++;
	}
	return (count);
}

static int	ft_free_arr(char **res, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (1);
}

static struct s_things	ft_do_the_split(struct s_things x, char **res,
	const char *s, int	*err)
{
	while (s[x.i] == x.c && s[x.i])
		x.i++;
	x.beg = x.i;
	while (s[x.i] != '\0')
	{
		if (s[x.i] == x.c)
		{
			res[x.j++] = ft_substr(s, x.beg, x.i - x.beg);
			if (!res[x.j - 1])
			{
				*err = ft_free_arr(res, x.j);
				return (x);
			}
			while (s[x.i] == x.c)
				x.i++;
			x.beg = x.i;
		}
		else
			x.i++;
	}
	return (x);
}

char	**ft_split(char const *s, char c)
{
	char			**res;
	struct s_things	x;
	int				err;

	x.i = 0;
	x.j = 0;
	err = 0;
	x.c = c;
	res = (char **)malloc((ft_count_words(s, x.c) + 1) * sizeof(char *));
	if (!res || !s)
		return (free(res), NULL);
	x = ft_do_the_split(x, res, s, &err);
	if (err)
		return (NULL);
	if (s[x.beg] != '\0')
	{
		res[x.j++] = ft_substr(s, x.beg, x.i - x.beg);
		if (!res[x.j - 1])
		{
			ft_free_arr(res, x.j);
			return (NULL);
		}
	}
	res[x.j] = 0;
	return (res);
}
