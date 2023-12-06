/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 08:34:01 by rluari            #+#    #+#             */
/*   Updated: 2023/12/05 20:55:08 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
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
		return (NULL);
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
	return (res);
}
