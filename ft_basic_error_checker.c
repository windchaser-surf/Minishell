/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_basic_error_checker.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 12:12:24 by rluari            #+#    #+#             */
/*   Updated: 2024/01/09 08:57:08 by codespace        ###   ########.fr       */
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

_Bool	ft_emptyness_in_cmd(char *cmd, int i)
{
	_Bool	prev_was_pipe;

	prev_was_pipe = 1;
	ft_skip_spaces(cmd, &i);
	if (cmd[i] == '\0')
		return (1);
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

_Bool	ft_basic_error_checker(char **command, int *error_code)
{
	char	*attach_to_end;
	char	*new_command;

	if (ft_unmatched_quotes(*command, error_code))
		return (1);
	if (ft_emptyness_in_cmd(*command, 0) == 1)	//check for emptyness between pipes, incl beginning
		return (free(*command), 1);
	if (ft_ends_with_spec(*command, error_code) == 2)	//ends with redirection character
		return (1);
	new_command = NULL;
	while(ft_ends_with_spec(*command, error_code) == 1)	//ends with pipe
	{
		free(new_command);
		attach_to_end = readline("> ");
		add_history(attach_to_end);
		new_command = ft_strjoin(*command, attach_to_end);
		free(attach_to_end);
		free(*command);
		*command = malloc(ft_strlen(new_command) + 1);
		if (*command == NULL)
			return (perror("Malloc failed"), free(new_command), 1);
		ft_strcpy(*command, new_command);
	}
	free(new_command);
	return (0);
}
