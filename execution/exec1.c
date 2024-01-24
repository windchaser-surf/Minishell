/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 11:17:48 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/24 16:27:46 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_process(t_parser *command, t_list **env_copy)
{
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
}

void	cmd_path_null(t_parser *command)
{
	char	*tmp;

	if (command->cmd_args == NULL)
	{
		ft_file_closer_single(command);
		g_ec = EXIT_SUCCESS;
		return ;
	}
	tmp = ft_strjoin(command->cmd_args[0], ": command not found\n");
	if (tmp == NULL)
	{
		perror("malloc: ");
		ft_file_closer_single(command);
		g_ec = MALLOC_ERR;
		return ;
	}
	ft_putstr_fd(tmp, STDERR_FILENO);
	if (tmp != NULL)
		free(tmp);
	ft_file_closer_single(command);
	g_ec = CMD_NOT_FOUND;
}

void	waiting_for(t_pipex *data)
{
	int	status;

	status = 0;
	waitpid(data->pid[0], &status, 0);
	if (WIFEXITED(status))
		g_ec = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		g_ec = WTERMSIG(status) + 128;
		ft_putchar_fd('\n', 2);
	}
}

void	exec_path(t_parser *command, t_list **env_copy)
{
	t_pipex	data;

	data.pid = (int *)malloc(sizeof(int));
	if (data.pid == NULL)
	{
		perror("malloc: ");
		g_ec = MALLOC_ERR;
		return ;
	}
	ft_init_signals(NOT_INPUT);
	data.pid[0] = fork();
	if (data.pid[0] == -1)
	{
		perror(command->cmd_args[0]);
		g_ec = EXIT_FAILURE;
		return ;
	}
	if (data.pid[0] == 0)
		child_process(command, env_copy);
	else
		waiting_for(&data);
	free(data.pid);
	ft_file_closer_single(command);
}

void	one_execution(t_parser *command, t_list **env_copy, \
	t_list *tokens)
{
	if (command->exit_code != 0)
	{
		ft_file_closer_single(command);
		g_ec = command->exit_code;
		return ;
	}
	if (command->cmd_path == NULL)
		return (cmd_path_null(command));
	else if (check_builtin(command->cmd_args[0]))
		return (exec_builtins(command, env_copy, tokens));
	else
		return (exec_path(command, env_copy));
}
