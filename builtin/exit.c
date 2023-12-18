/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 17:40:12 by fwechsle          #+#    #+#             */
/*   Updated: 2023/12/18 09:52:42 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* int convert_number(char *arg)
{
	//auch minus zahlen müssen richt converted werden und wenn sie 9223372036854775807 größer sind dann ist es außerhalb der 
	//Max long long range und zählt dann nicht mehr als numerischer parameter
} */

int    builtin_exit(char **arg)
{
	int i;
	int check;
	int	n;

	check = 0;
	i = 0; 
	while (arg[i])
		i++;
	if ( i == 1)
	{
		printf("exit\n");
		exit (0);
	}
	n = 0;
	while (arg[1][n])
	{
		if (arg[1][n] < '0' || arg[1][n] > '9')
			check = 1;
		n++;
	}
	if (check == 0 && i > 2)
	{
		ft_putstr_fd("exit\nexit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else if (check == 1)
	{
		ft_putstr_fd("exit\nexit: needs a numeric parameter\n", STDERR_FILENO);
		exit (2);
	}
	else if (check == 0 && i == 2)
	{
		printf("exit\n");
		exit (0); //Zahl muss noch richtig umgerechnet werden
	}
	return (EXIT_SUCCESS);
}