/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 12:59:26 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 13:07:12 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_cmd_name(char *cmd_with_path)
{
	int	i;

	i = ft_strlen(cmd_with_path) - 1;
	while (i >= 0)
	{
		if (cmd_with_path[i] == '/')
			return (ft_strdup(cmd_with_path + i + 1));
		i--;
	}
	return (NULL);
}

_Bool	ft_is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	else
		return (0);
}

char	*ft_cut_until_equal(char *str)
{
	int		i;
	char	*new_str;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	new_str = ft_substr(str, 0, i);
	if (new_str == NULL)
		return (NULL);
	return (new_str);
}
	
//handle absoulte and relative path, jani/ls is wrong, ls is correct, /user/bin/ls is correct
//the first WORD is the program name, every other word in order vecomes a parameter
//if the command (first WORD) doesnt exist or wrong path if given by /, then set exit_error to 1


int is_directory(const char *path)
{
    struct stat path_stat;
	// Get information about the file or directory
	if (stat(path, &path_stat) == 0)
	{
		// Check if it's a directory
		if (S_ISDIR(path_stat.st_mode))
			return 1;  // It's a directory
		else
			return 0;  // It's not a directory (could be a file)
	}
	else
		return(-1);
}

_Bool	ft_set_exit_err_empty_arg(t_parser **parser_node, int exit_code)
{
	char	*ec;

	if ((*parser_node) == NULL)
		return (1);
	if ((*parser_node)->exit_code != 0)
		return (0);
	if ((*parser_node)->cmd_args == NULL)
		return (0);
	if (ft_strcmp((*parser_node)->cmd_args[0], "exit") != 0
		|| (*parser_node)->cmd_args[1] != NULL)
		return (0);
	ec = ft_itoa(exit_code);
	if (!ec)
		return (perror("Malloc failed"), 1);
	(*parser_node)->cmd_args = ft_realloc_array((*parser_node)->cmd_args, ec);
	free(ec);
	return (0);
}