/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_basic_error_checker.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 12:12:24 by rluari            #+#    #+#             */
/*   Updated: 2024/01/23 11:21:47 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_ends_with_spec(char *command, int *error_code)
{
	int	i;

	i = ft_strlen(command) - 1;
	while (command[i] == ' ')
		i--;
	if (command[i] == '|')
		return (1);
	else if (command[i] == '<' || command[i] == '>')
	{
		ft_putstr_fd(EMSG_SYN, 2);
		ft_putstr_fd("newline'\n", 2);
		return (free(command), *error_code = 2, 2);
	}
	return (0);
}

_Bool	ft_emptyness_in_cmd(char *cmd, int i, int *e_c, _Bool prev_was_pipe)
{
	ft_skip_spaces(cmd, &i);
	if (cmd[i] == '\0')
		return (*e_c = 2, 1);
	while (cmd[i])
	{
		ft_skip_spaces(cmd, &i);
		if (cmd[i] == '"' || cmd[i] == '\'')
		{
			ft_skip_to_closing_quote(cmd, &i, cmd[i]);
			prev_was_pipe = 0;
			continue ;
		}
		else if (cmd[i] == '|' && prev_was_pipe == 1)
			return (*e_c = 2, ft_putstr_fd(EMSG_SYN, 2),
				ft_putstr_fd("|'\n", 2), 1);
		else if (cmd[i] == '|' && prev_was_pipe == 0)
			prev_was_pipe = 1;
		else
			prev_was_pipe = 0;
		if (cmd[i] != '\0')
			i++;
	}
	return (0);
}

int	ft_valid_red(char *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i] && (cmd[i] == '<' || cmd[i] == '>'))
		{
			i++;
			j++;
		}
		if (j > 2)
			return (1);
		i++;
	}
	return (0);
}

_Bool	ft_ends_with_pipe(char **command, char *new_command)
{
	char	*attach_to_end;

	new_command = NULL;
	free(new_command);
	attach_to_end = readline("> ");
	if (!attach_to_end)
		return (free(*command), 1);
	add_history(attach_to_end);
	new_command = ft_strjoin(*command, attach_to_end);
	if (attach_to_end[0] == EOF)
		return (g_ec = 2, 0);
	free(attach_to_end);
	free(*command);
	*command = malloc(ft_strlen(new_command) + 1);
	if (*command == NULL)
		return (ft_putstr_fd(EMSG_MAL, 2), free(new_command), 1);
	ft_strcpy(*command, new_command);
	free(new_command);
	return (0);
}

_Bool	ft_basic_error_checker(char **command, int *exit_code)
{
	char	*new_command;

	new_command = NULL;
	if (ft_unmatched_quotes(*command, exit_code))
		return (1);
	if (ft_emptyness_in_cmd(*command, 0, exit_code, 1) == 1)
		return (free(*command), 1);
	if (ft_ends_with_spec(*command, exit_code) == 2)
		return (1);
	if (ft_valid_red(*command))
		return (1);
	ft_init_signals(HEREDOC_INP);
	while (ft_ends_with_spec(*command, exit_code) == 1)
	{
		if (ft_ends_with_pipe(command, new_command) == 1 || *exit_code > 0)
			return (1);
	}
	free(new_command);
	return (0);
}
