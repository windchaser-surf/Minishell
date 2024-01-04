
/*
	Checks for basic errors in the command line.
	1. Checks for unmatched single quotes.
	2. Checks for unmatched double quotes.
*/

#include "libft/libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <stdbool.h>

_Bool	ft_unmatched_quotes(char *command)	// 1 if error, 0 if correct
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
				return (1);
		}
		i++;
	}
	return (0);	
}

int	ft_ends_with_spec(char *command)
{
	int	i;

	i = ft_strlen(command) - 1;
	while (command[i] == ' ')
		i--;
	if (command[i] == '|')
		return (1);
	else if (command[i] == '<' || command[i] == '>')
	{
		return (2);
	}
	return (0);
}

_Bool	ft_is_empty_command(char *command)
{
	int	i;

	i = 0;
	while (command[i])
	{
		if (command[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

_Bool	ft_basic_error_checker(char **command, int *error_code)	//handle if "'''''" stb
{
	int		i;
	int		len;
	char	*attach_to_end;
	char	*new_command;

	new_command = NULL;
	len = ft_strlen(*command);
	i = len - 1;
	(void)i;
	(void)len;
	if (ft_is_empty_command(*command))
		return (free(*command), 1);
	if (ft_unmatched_quotes(*command))
		return (ft_putstr_fd("Minishell: syntax error: Unmatched quotes\n", 2), *error_code = 2, free(*command), 1);
	while(ft_ends_with_spec(*command) == 1)	//ends with pipe
	{
		free(new_command);
		attach_to_end = readline("> ");
		add_history(attach_to_end);
		new_command = ft_strjoin(*command, attach_to_end);
		free(attach_to_end);
		free(*command);
		*command = malloc(ft_strlen(new_command) + 1);
		if (*command == NULL)
			return (perror("Malloc failed"), 1);
		ft_strcpy(*command, new_command);
		printf("command: %s\n", *command);
	}
	free(new_command);
	if (ft_ends_with_spec(*command) == 2)	//ends with redirection character
		return (ft_putstr_fd("Minishell: syntax error near unexpected token `newline'\n", 2), *error_code = 2, free(*command), 1);
	
	return (0);
}