/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:03:55 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/23 18:06:03 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_builtins(t_parser *command, t_list **env_copy, \
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
			g_ec = EXIT_FAILURE;
			return ;
		}
		close (command->fd_out);
	}
	return (run_builtins_parent(command, env_copy, tokens));
}
