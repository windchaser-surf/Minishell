/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 16:49:09 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 20:52:55 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;
	int		full_len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	full_len = ft_strlen(s1) + ft_strlen(s2);
	res = (char *)malloc(full_len + 1);
	if (!res)
		return (free(s1), NULL);
	while (i < (int)ft_strlen(s1))
	{
		res[i] = s1[i];
		i++;
	}
	while (i < full_len)
	{
		res[i] = s2[j];
		i++;
		j++;
	}
	res[i] = '\0';
	free(s1);
	return (res);
}
