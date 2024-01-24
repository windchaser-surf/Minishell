/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:17:42 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/24 16:27:00 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cmd_not_found(t_parser *command, t_list *tokens, t_pipex *data, \
	t_list **env_copy)
{
	char	*tmp;

	if (command->cmd_args == NULL)
	{
		ft_file_closer_single(command);
		ft_free_child(data, tokens, env_copy);
		exit (EXIT_SUCCESS);
	}
	tmp = ft_strjoin(command->cmd_args[0], ": command not found\n");
	if (tmp == NULL)
	{
		perror("malloc: ");
		ft_free_child(data, tokens, env_copy);
		exit (MALLOC_ERR);
	}
	ft_putstr_fd(tmp, STDERR_FILENO);
	free(tmp);
	ft_file_closer_single(command);
	ft_free_child(data, tokens, env_copy);
	exit (CMD_NOT_FOUND);
}

void	ft_file_closer_single(t_parser *command)
{
	if (command->fd_in != -1)
	{
		close (command->fd_in);
		command->fd_in = -1;
	}
	if (command->fd_out != -1)
	{
		close (command->fd_out);
		command->fd_out = -1;
	}
}
