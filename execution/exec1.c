/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 11:17:48 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/24 09:43:50 by rluari           ###   ########.fr       */
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

void	ft_sighandle_childd(int sig)
{
	if (sig == SIGINT)
	{
		g_ec = 130;
		printf("exit code: %d\n", g_ec);
		rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        write(STDOUT_FILENO, "\n", 1);
	}
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
	signal(SIGINT, &ft_sighandle_heredoc);
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

void	exec_path(t_parser *command, t_list **env_copy)
{
	t_pipex	data;
	int		status;

	status = 0;
	data.pid = (int *)malloc(sizeof(int));
	if (data.pid == NULL)
	{
		perror("malloc: ");
		g_ec = MALLOC_ERR;
		return ;
	}
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
		waitpid(data.pid[0], &status, 0);
	ft_init_signals(NOT_INPUT);
	status = WEXITSTATUS(status);
	free(data.pid);
	printf("exit code: %d\n", g_ec);
	ft_file_closer_single(command);
	if (g_ec != 130)
		g_ec = status;
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
