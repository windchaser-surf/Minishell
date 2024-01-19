/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:03:55 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 18:21:59 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_builtins(t_parser *command, t_list **env_copy, int exit_code, \
	t_list *tokens)
{
	if (command->fd_in != -1)
	{
		close (command->fd_in);
		command->fd_in = -1;
	}
	if (command->fd_out != -1)
	{
		command->fd_in = dup(STDOUT_FILENO);
		if (dup2(command->fd_out, 1) == -1)
		{
			perror("dup: ");
			ft_file_closer_single(command);
			return (EXIT_FAILURE);
		}
		close (command->fd_out);
	}
	return (run_builtins_parent(command, env_copy, exit_code, tokens));
}
