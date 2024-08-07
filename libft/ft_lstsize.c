/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luari <rluari@student.42vienna.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 12:42:44 by luari             #+#    #+#             */
/*   Updated: 2023/09/15 14:46:46 by luari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	if (lst)
	{
		i = 1;
		while ((*lst).next != NULL)
		{
			lst = (*lst).next;
			i++;
		}
		return (i);
	}
	return (0);
}
