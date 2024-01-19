/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 11:17:48 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 18:05:47 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* int	exec_builtins(t_parser *command, t_list **env_copy, int exit_code, \
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
} */

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

int	child_process(t_parser *command, t_list **env_copy)
{
	ft_init_signals(CHILD);
	if (command->heredoc)
		dup_heredoc(command);
	else if (command->fd_in != -1)
	{
		if (dup2(command->fd_in, 0) == -1)
		{
			perror("dup2: ");
			ft_file_closer_single(command);
			exit(EXIT_FAILURE);
		}
	}
	if (command->fd_out != -1)
	{
		if (dup2(command->fd_out, 1) == -1)
		{
			perror("dup2: ");
			ft_file_closer_single(command);
			exit(EXIT_FAILURE);
		}
	}
	ft_file_closer_single(command);
	execution(command, *env_copy);
	return (EXIT_SUCCESS);
}

int	cmd_path_null(t_parser *command)
{
	char	*tmp;

	if (command->cmd_args == NULL)
	{
		ft_file_closer_single(command);
		return (EXIT_SUCCESS);
	}
	tmp = ft_strjoin(command->cmd_args[0], ": command not found\n");
	if (tmp == NULL)
	{
		perror("malloc: ");
		ft_file_closer_single(command);
		return (MALLOC_ERR);
	}
	ft_putstr_fd(tmp, STDERR_FILENO);
	if (tmp != NULL)
		free(tmp);
	ft_file_closer_single(command);
	return (CMD_NOT_FOUND);
}

int	exec_path(t_parser *command, t_list **env_copy)
{
	t_pipex	data;
	int		status;

	status = 0;
	data.pid = (int *)malloc(sizeof(int));
	if (data.pid == NULL)
	{
		perror("malloc: ");
		return (MALLOC_ERR);
	}
	data.pid[0] = fork();
	if (data.pid[0] == -1)
	{
		perror(command->cmd_args[0]);
		return (EXIT_FAILURE);
	}
	if (data.pid[0] == 0)
		child_process(command, env_copy);
	else
		waitpid(data.pid[0], &status, 0);
	status = WEXITSTATUS(status);
	free(data.pid);
	ft_file_closer_single(command);
	return (status);
}

int	one_execution(t_parser *command, t_list **env_copy, int exit_code, \
	t_list *tokens)
{
	if (command->exit_code != 0)
	{
		ft_file_closer_single(command);
		return (command->exit_code);
	}
	if (command->cmd_path == NULL)
		return (cmd_path_null(command));
	else if (check_builtin(command->cmd_args[0]))
		return (exec_builtins(command, env_copy, exit_code, tokens));
	else
		return (exec_path(command, env_copy));
}
