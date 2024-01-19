/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 12:56:15 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 19:31:02 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_init_parser_node(t_parser **parser_node)
{
	(*parser_node)->cmd_path = NULL;
	(*parser_node)->cmd_args = NULL;
	(*parser_node)->fd_in = -1;
	(*parser_node)->fd_out = -1;
	(*parser_node)->exit_code = 0;
	(*parser_node)->heredoc = NULL;
	(*parser_node)->heredoc_tmp = NULL;
}

void	ft_free_parser(t_list *parser_head)
{
	t_list		*tmp;
	t_parser	*parser_node;

	while (parser_head)
	{
		tmp = parser_head->next;
		parser_node = (t_parser *)parser_head->content;
		if (parser_node->cmd_path)
		{
			if (ft_strcmp(parser_node->cmd_path, "BUILTIN"))
				free(parser_node->cmd_path);
		}
		if (parser_node->cmd_args)
			ft_free_array(parser_node->cmd_args);
		free(parser_node->heredoc);
		free(parser_node->heredoc_tmp);
		free(parser_head->content);
		free(parser_head);
		parser_head = tmp;
	}
}

void	ft_free_p_node(t_parser **parser_node)
{
	if (!(*parser_node))
		return ;
	if ((*parser_node)->cmd_path
		&& ft_strcmp((*parser_node)->cmd_path, "BUILTIN") != 0)
		free((*parser_node)->cmd_path);
	ft_free_array((*parser_node)->cmd_args);
	(*parser_node)->cmd_args = NULL;
	free((*parser_node)->heredoc);
	free((*parser_node)->heredoc_tmp);
}

void	ft_perror_and_free(char *str)
{
	char	*asd;

	asd = ft_strjoin("Minishell: ", str);
	perror(asd);
	free(asd);
}

int	get_pos_of_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}
