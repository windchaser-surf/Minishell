/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_basic_error_checker.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 12:12:24 by rluari            #+#    #+#             */
/*   Updated: 2024/01/18 21:58:10 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

_Bool	ft_unmatched_quotes(char *command, int *error_code)	// 1 if error, 0 if correct
{
	int		i;
	char	quote;

	i = 0;
	while (command[i])
	{
		if (command[i] == '\'' || command[i] == '\"')
		{
			quote = command[i];
			i++;
			while (command[i] && command[i] != quote)
				i++;
			if (command[i] == '\0')
			{
				ft_putstr_fd("Minishell: syntax error: Unmatched quotes\n", 2);
				return (*error_code = 2, 1);
			}
		}
		i++;
	}
	return (0);
}

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
		ft_putstr_fd("Minishell: syntax error near unexpected token `newline'\n", 2);
		return (free(command), *error_code = 2, 2);
	}
	return (0);
}

_Bool	ft_emptyness_in_cmd(char *cmd, int i, int *error_code)
{
	_Bool	prev_was_pipe;

	prev_was_pipe = 1;
	ft_skip_spaces(cmd, &i);
	if (cmd[i] == '\0')
		return (*error_code = 2, 1);
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
			return (ft_putstr_fd("Minishell: syntax error near unexpected token `|'\n", 2), 1);
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
	int j;

	i = 0;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i] && (cmd[i] == '<' || cmd[i] =='>'))
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

_Bool	ft_basic_error_checker(char **command, int *exit_code)
{
	char	*attach_to_end;
	char	*new_command;

	if (ft_unmatched_quotes(*command, exit_code))
		return (1);
	
	if (ft_emptyness_in_cmd(*command, 0, exit_code) == 1)	//check for emptyness between pipes, incl beginning
		return (free(*command), 1);
	if (ft_ends_with_spec(*command, exit_code) == 2)	//ends with redirection character
		return (1);
	if (ft_valid_red(*command))
		return (1);
	new_command = NULL;
	ft_init_signals(INPUT);
	while(ft_ends_with_spec(*command, exit_code) == 1)	//ends with pipe
	{
		free(new_command);
		attach_to_end = readline("> ");
		if (!attach_to_end)
			return (free(*command), 1);
		add_history(attach_to_end);
		new_command = ft_strjoin(*command, attach_to_end);
		free(attach_to_end);
		free(*command);
		*command = malloc(ft_strlen(new_command) + 1);
		if (*command == NULL)
			return (perror("Malloc failed"), free(new_command), 1);
		ft_strcpy(*command, new_command);
	}
	ft_init_signals(NOT_INPUT);

	free(new_command);
	return (0);
}
