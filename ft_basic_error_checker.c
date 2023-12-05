
/*
	Checks for basic errors in the command line.
	1. Checks for unmatched single quotes.
	2. Checks for unmatched double quotes.
*/

#include "minishell.h"

char *ft_getenv(const char *name)
{
	(void)name;
	return ("temp");	//TODO: implement this function
}

_Bool	ft_check_existence_of_var(char *var, int end)	// 1 if exists, 0 if not
{
	char	*var_name;
	char	*var_value;

	var_name = ft_substr(var, 0, end);
	var_value = ft_getenv(var_name);
	if (var_value == NULL)
		return (0);
	return (1);
}

int	ft_variable_syntax_check(char *var, int *i)	// 1 if error, 0 if correct. A variable name must consist of only alphanumeric characters and underscores.
{
	int	j;

	j = 0;
	if (var[j] == '\0' || var[j] == ' ')
		return (ft_putstr_fd("Error: Expected variable name", 2), 1);
	j++;
	while (var[j])
	{
		if (!ft_isalnum(var[j]) && var[j] != '_')
			return (1);
		j++;
	}
	*i += j;
	if (ft_check_existence_of_var(var, j) == 0)
		return (ft_putstr_fd("Error: Variable does not exist", 2), 1);
	return (0);
}

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

void normalizeMetacharacters(char *str) {
    // Define the list of metacharacters
    const char *metacharacters = ".*+?()[]{}^$|\\";

    // Iterate through each character in the string
    for (int i = 0; i < (int)ft_strlen(str); i++) {
        // Check if the current character is a metacharacter
        if (ft_strchr(metacharacters, str[i]) != NULL)
		{
            // If it is a metacharacter, replace it with its escaped form
            char replacement[3] = {'\\', str[i], '\0'};
            ft_memmove(&str[i + 2], &str[i + 1], ft_strlen(str) - i);  // Make room for the replacement
            ft_strncpy(&str[i], replacement, 2);  // Copy the replacement
            i += 1;  // Move the index forward to skip the next character
        }
    }
}

_Bool	ft_basic_error_checker(char *command)	//handle if "'''''" stb
{
	int		i;

	i = 0;
	if (ft_unmatched_quotes(command))
		return (1);

	return (0);
}