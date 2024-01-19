/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 12:59:26 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 19:31:08 by rluari           ###   ########.fr       */
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

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
			return (1);
		else
			return (0);
	}
	else
		return (-1);
}

_Bool	ft_make_command(t_parser_h *h, _Bool last)
{
	if (h->error)
		return (ft_free_parser(h->p_list_head), 1);
	if (h->i_cmd != -1 || last == 1)
	{
		h->p_new_node = ft_lstnew(h->parser_n);
		if (h->p_new_node == NULL)
			return (ft_free_parser(h->p_list_head),
				ft_free_p_node(&h->parser_n), ft_putstr_fd(EMSG_MAL, 2), 1);
		ft_lstadd_back(&h->p_list_head, h->p_new_node);
	}
	if (last == 0)
	{
		h->parser_n = (t_parser *)malloc(sizeof(t_parser));
		if (h->parser_n == NULL)
			return (ft_free_parser(h->p_list_head),
				ft_putstr_fd(EMSG_MAL, 2), 1);
		ft_init_parser_node(&(h->parser_n));
		h->i_cmd = h->lexed_i->exec_num;
		h->prev_was_word = 0;
	}
	return (0);
}
