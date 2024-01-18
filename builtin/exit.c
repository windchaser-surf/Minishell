/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 17:40:12 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/18 11:26:06 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* int convert_number(char *arg)
{
	//auch minus zahlen müssen richt converted werden und wenn sie 9223372036854775807 größer sind dann ist es außerhalb der 
	//Max long long range und zählt dann nicht mehr als numerischer parameter
} */
long long	ft_atoi_long(const char *str)
{
	int			i;
	long long	result;
	long long	pre;

	pre = 1;
	i = 0;
	result = 0;
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			pre *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * pre);
}

int check_for_number(char *arg)
{
	int	n;
	
	n = 0;
	if (arg[0] == '-' || arg[0] =='+')
		n++;
	while (arg[n])
	{
		if ((arg[n] < '0' || arg[n] > '9'))
			return (1);
		n++;
	}
	return (0);
}
//if pid_check == 1 => child_process
int exit_too_many(int pid_check)
{
	if (pid_check == 1)
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
	else	
		ft_putstr_fd("exit\nexit: too many arguments\n", STDERR_FILENO);
	return (1);
}

int exit_not_numeric(int pid_check, t_list *tokens, t_list **env_copy)
{
	if (pid_check == 1)
	{
		ft_putstr_fd("exit: numeric argumend required\n", STDERR_FILENO);
		return (2);
	}
	else
	{
		ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO);
		if (env_copy)
			ft_lstclear(env_copy, free);
		rl_clear_history();
	}
	if (tokens)
		ft_free_parser(tokens);
	exit (2);
}

int exit_with_number(int pid_check, char *numb, t_list *tokens, t_list **env_copy) //hier kommt noch die nummer hin
{
	unsigned char c;
	
	if (pid_check == 0)
	{
		printf("exit\n");
		if (env_copy)
			ft_lstclear(env_copy, free);
		rl_clear_history();
	}
	c = ft_atoi_long(numb);
	if (pid_check == 1)
		return (c);
	if (tokens)
		ft_free_parser(tokens);
	exit (c); //Zahl muss noch richtig umgerechnet werden
}

int    builtin_exit(char **arg, int exit_code, int pid_check)
{
	int i;
	int check;

	check = 0;
	i = 0; 
	while (arg[i])
		i++;
	if ( i == 1)
	{
		if (pid_check == 0)
			printf("exit\n");
		exit (exit_code);
	}
	check = check_for_number(arg[1]);
	if (check == 0 && i > 2)
		return(exit_too_many(pid_check));
	else if (check == 1)
		return (exit_not_numeric(pid_check, NULL, NULL));
	else if (check == 0 && i == 2)
		return (exit_with_number(pid_check, arg[1], NULL, NULL));
	return (EXIT_SUCCESS);
}

int    builtin_exit_parent(char **arg, int exit_code, t_list *tokens, t_list **env_copy)
{
	int i;
	int check;

	check = 0;
	i = 0; 
	while (arg[i])
		i++;
	if ( i == 1) // kann gelöscht werden muss aber auch noch exit_code für unused variable entfernt werden
	{
		printf("exit\n");
		ft_free_parser(tokens);
		exit (exit_code);
	}
	check = check_for_number(arg[1]);
	if (check == 0 && i > 2)
		return(exit_too_many(0));
	else if (check == 1)
		exit_not_numeric(0, tokens, env_copy);
	else if (check == 0 && i == 2)
		exit_with_number(0, arg[1], tokens, env_copy);
	return (EXIT_SUCCESS);
}
