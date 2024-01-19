/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:59:46 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 21:43:57 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

_Bool	ft_cmd_is_dot(char *cmd, t_parser **parser_node)
{
	if (cmd[0] == '.' && strlen(cmd) == 2)
	{
		if (cmd[1] == '.')
			return (1);
	}
	else if (cmd[0] == '.' && strlen(cmd) == 1)
	{
		ft_putstr_fd("Minishell: .: filename argument required\n", 2);
		return ((*parser_node)->exit_code = 127, 1);
	}
	return (0);
}

void	ft_init_parser_helper_struct(t_parser_h *helper)
{
	helper->error = 0;
	helper->i_cmd = -1;
	helper->p_list_head = NULL;
	helper->p_new_node = NULL;
	helper->parser_n = NULL;
	helper->prev_was_word = 0;
	helper->lexed_i = NULL;
}

_Bool	ft_is_empty_lexed_lode(t_list **lexed_list)
{
	t_lexer	*lexed_i;

	lexed_i = (t_lexer *)(*lexed_list)->content;
	if (lexed_i->wasnt_empty_var == 0)
		return (*lexed_list = (*lexed_list)->next, 1);
	return (0);
}

_Bool	ft_unmatched_quotes(char *command, int *error_code)
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
