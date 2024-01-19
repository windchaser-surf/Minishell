/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:16:41 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 18:22:31 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pipe_closer(t_pipex *data)
{
	int	i;

	i = 0;
	while (i < (data->nbr_p * 2))
	{
		close(data->p[i]);
		i++;
	}
}

void	error_closing(t_parser *command, t_pipex *data, t_list *tokens, \
	t_list **env_copy)
{
	ft_file_closer_single(command);
	ft_free_child(data, tokens, env_copy);
	exit(EXIT_FAILURE);
}

void	dup_input(t_parser *command, t_pipex *data, t_list *tokens, \
	t_list **env_copy)
{
	if (command->exit_code != 0)
		error_closing(command, data, tokens, env_copy);
	if (command->heredoc)
		dup_heredoc(command);
	else if (command->fd_in != -1)
	{
		if (dup2(command->fd_in, 0) == -1)
		{
			perror("dup2: ");
			error_closing(command, data, tokens, env_copy);
		}
		close(command->fd_in);
		command->fd_in = -1;
	}
	else
	{
		if (data->n2 != 0)
		{
			if (dup2(data->p[(data->n2 * 2) - 2], 0) == -1)
			{
				perror("dup2: ");
				error_closing(command, data, tokens, env_copy);
			}
		}
	}
}

void	ft_free_child(t_pipex *data, t_list *tokens, t_list **env_copy)
{
	t_list	*tmp;

	tmp = tokens;
	while (tmp)
	{
		ft_file_closer_single((t_parser *)(tmp->content));
		tmp = tmp -> next;
	}
	ft_free_parser(tokens);
	free (data->pid);
	ft_pipe_closer(data);
	free (data->p);
	ft_lstclear (env_copy, free);
}

void	dup_output(t_parser *command, t_pipex *data, t_list *tokens, \
	t_list **env_copy)
{
	if (command->fd_out != -1)
	{
		if (dup2(command->fd_out, 1) == -1)
		{
			perror("dup2 ");
			error_closing(command, data, tokens, env_copy);
		}
		close(command->fd_out);
		command->fd_out = -1;
	}
	else
	{
		if (data->n2 < data->nbr_p)
		{
			if (dup2(data->p[1 + (2 * data->n2)], 1) == -1)
			{
				perror("dup2 ");
				error_closing(command, data, tokens, env_copy);
			}
		}
	}
}
