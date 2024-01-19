/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:17:32 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 16:40:04 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_echo_builtin(char **arg)
{
	int	i;
	int	option;

	option = 0;
	i = 1;
	if (!ft_strncmp(arg[1], "-n", 3))
	{
		option = 1;
		i++;
	}
	while (arg[i])
	{
		ft_putstr_fd(arg[i], 1);
		if (arg[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
	if (option == 0)
		write(1, "\n", 1);
	return (EXIT_SUCCESS);
}
