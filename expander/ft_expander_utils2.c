/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 20:31:47 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/23 11:00:15 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*ft_lexer_but_with_words_and_one_cmd(char *command, int cmd_num)
{
	t_lexer_helper	helper;

	ft_init_lexer_helper(&helper, command, cmd_num);
	while (command[helper.i])
	{
		if (command[helper.i] == ' ')
		{
			if (ft_handle_lexer_word(&helper, command, NULL))
				return (NULL);
		}
		else
			helper.i++;
	}
	if (helper.start != helper.i)
	{
		if (ft_make_lnode(&helper, command))
			return (NULL);
	}
	return (helper.list_head);
}

char	*ft_concat_rest(char *str, t_ex_h *h, char *new_str, \
	_Bool needs_expansion)
{
	char	*rest;

	if (needs_expansion)
		h->i = ft_strlen(new_str);
	rest = ft_substr(str, h->orig_i + h->vns, \
		ft_strlen(str) - (h->orig_i + h->vns));
	if (!rest)
		return (ft_putstr_fd(EMSG_MAL, 2), NULL);
	new_str = ft_strjoin_free(new_str, rest);
	free(rest);
	return (new_str);
}

char	*ft_attach_beginning(char *head_node_str, char *str, \
	t_ex_h *h)
{
	char	*tmp;

	tmp = malloc(sizeof(char) * ft_strlen(head_node_str) + h->orig_i + 1);
	if (!tmp)
		return (perror("Malloc failed"), NULL);
	ft_strlcpy(tmp, str, (size_t)h->orig_i + 1);
	ft_strlcat(tmp, head_node_str, ft_strlen(head_node_str) + h->orig_i + 1);
	free(head_node_str);
	return (tmp);
}

_Bool	ft_is_var_char(char c)
{
	if (c == '\0')
		return (0);
	if (ft_isalnum(c) || c == '_' || c == '?')
		return (1);
	return (0);
}

char	*ft_checker_for_expand_with_split(t_ex_h *h, char *new_str, \
	t_lexer *orig_lex_node)
{
	t_list	*new_nodes_head;

	new_nodes_head = NULL;
	if (h->needs_expansion == 1)
	{
		new_nodes_head = ft_lexer_but_with_words_and_one_cmd(new_str + \
			h->orig_i, orig_lex_node->exec_num);
		if (h->orig_i > 0)
			((t_lexer *)(new_nodes_head)->content)->word = \
				ft_attach_beginning(((t_lexer *)(new_nodes_head)->content)-> \
					word, orig_lex_node->word, h);
		free(new_str);
		((t_lexer *)(ft_lstlast(new_nodes_head))->content)->word = \
			ft_concat_rest(orig_lex_node->word, h, \
				((t_lexer *)(ft_lstlast(new_nodes_head))->content)->word, \
					h->needs_expansion);
		return (ft_insert_new_lexed_nodes(new_nodes_head, h));
	}
	else
		return (new_str = ft_concat_rest(orig_lex_node->word, h, new_str, 0), \
			free(orig_lex_node->word), new_str);
}
