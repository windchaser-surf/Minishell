/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 09:01:54 by rluari            #+#    #+#             */
/*   Updated: 2024/01/18 20:00:00 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	j = ft_strlen(s1);
	while (s1[i] && ft_strchr(set, s1[i]) != NULL && i < j)
		i++;
	while (s1[j - 1] && ft_strchr(set, s1[j - 1]) != NULL && i < j)
		j--;
	res = ft_substr(s1, i, j - i);
	if (!res)
		return (NULL);
	return (res);
}
