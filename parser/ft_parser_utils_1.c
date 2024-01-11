/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 12:42:09 by rluari            #+#    #+#             */
/*   Updated: 2024/01/11 14:42:28 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

_Bool	ft_str_has_quote(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

int	ft_count_amount_of_quotes(char *str)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			quotes++;
		i++;
	}
	return (quotes);
}

char	*ft_just_remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = ft_count_amount_of_quotes(str);
	new_str = (char *)malloc(sizeof(char) * (ft_strlen(str) - j + 1));
	if (new_str == NULL)
		return (NULL);
	j = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

/*_Bool	ft_ends_with_slash(char *str)
{
	int	i;

	i = ft_strlen(str) - 1;
	if (str[i] == '/')
		return (1);
	return (0);
}*/

int	ft_dir_accession(char *cmd, t_parser **parser_n)
{
	if (is_directory(cmd) == 1)
	{
		(*parser_n)->exit_code = 126;
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
	}
	else //is_directory(cmd) == 0
	{
		if (access(cmd, F_OK) == 0)
			return (0);
		else
			return ((*parser_n)->exit_code = 127, ft_perror_and_free(cmd), 1);
	}
	
	return (1);
}
