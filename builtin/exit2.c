/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:45:35 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 16:49:20 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_exit(char **arg, int exit_code, int pid_check)
{
	int	i;
	int	check;

	check = 0;
	i = 0;
	while (arg[i])
		i++;
	if (i == 1)
		return (exit_code);
	check = check_for_number(arg[1]);
	if (check == 0 && i > 2)
		return (exit_too_many(pid_check));
	else if (check == 1)
		return (exit_not_numeric(pid_check, NULL, NULL));
	else if (check == 0 && i == 2)
		return (exit_with_number(pid_check, arg[1], NULL, NULL));
	return (EXIT_SUCCESS);
}

int	builtin_exit_parent(char **arg, int exit_code, t_list *tokens, \
	t_list **env_copy)
{
	int	i;
	int	check;

	check = 0;
	i = 0;
	while (arg[i])
		i++;
	if (i == 1)
	{
		//printf("exit\n");
		ft_lstclear(env_copy, free);
		clear_history();
		ft_free_parser(tokens);
		exit (exit_code);
	}
	check = check_for_number(arg[1]);
	if (check == 0 && i > 2)
		return (exit_too_many(0));
	else if (check == 1)
		exit_not_numeric(0, tokens, env_copy);
	else if (check == 0 && i == 2)
		exit_with_number(0, arg[1], tokens, env_copy);
	return (EXIT_SUCCESS);
}
