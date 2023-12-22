/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felix <felix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:07:31 by fwechsle          #+#    #+#             */
/*   Updated: 2023/12/22 14:31:39 by felix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_builtin(char *str)
{
	if (!ft_strncmp(str, "cd", 3))
		return (1);
	else if (!ft_strncmp(str, "exit", 5))
		return (1);
	else if (!ft_strncmp(str, "pwd", 4))
		return (1);
	else if (!ft_strncmp(str, "echo", 5))
		return (1);
	else if (!ft_strncmp(str, "env", 4))
		return (1);
	else if (!ft_strncmp(str, "export", 7))
		return (1);
	else if (!ft_strncmp(str, "unset", 6))
		return (1);
	return (0);
}

int run_builtins(t_parser *command, t_list **env_copy, int error_code, int pid_check)
{
	int exit_code;
	
	exit_code = 0;
	if (!ft_strncmp(command->cmd_args[0], "cd",3))
		exit_code = cd_builtin(command->cmd_args[1], env_copy);
	else if (!ft_strncmp(command->cmd_args[0], "exit", 5))
		exit_code = builtin_exit(command->cmd_args, error_code, pid_check);
	else if (!ft_strncmp(command->cmd_args[0], "pwd", 4))
		exit_code = ft_pwd_builtin();
	else if (!ft_strncmp(command->cmd_args[0], "echo", 5))
		exit_code = ft_echo_builtin(command->cmd_args);
	else if (!ft_strncmp(command->cmd_args[0], "env", 4))
		exit_code = print_env(*env_copy);
	else if (!ft_strncmp(command->cmd_args[0], "export", 7))
		exit_code = export_builtin(command->cmd_args, env_copy);
	else if (!ft_strncmp(command->cmd_args[0], "unset", 6))
		exit_code = builtin_unset(env_copy, command->cmd_args);
	
	if (command->fd_in != -1)
	{
		dup2(command->fd_in, 1);	
		close(command->fd_in);
	}
	return (exit_code);
}