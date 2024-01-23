/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_handlers_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 19:47:16 by rluari            #+#    #+#             */
/*   Updated: 2024/01/22 14:45:49 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_init_expander_helper_nulls(t_ex_h *h, t_list **env_copy)
{
	h->list_head = NULL;
	h->current_node = h->list_head;
	h->env_copy = env_copy;
	h->i = 0;
	h->orig_i = 0;
	h->vns = 0;
	h->var_value = NULL;
	h->needs_expansion = 0;
}

char	*ft_free_for_expand_inline(char *new_str, char *str, \
	t_ex_h *h)
{
	free(h->var_value);
	h->var_value = NULL;
	free(str);
	str = NULL;
	return (new_str);
}

char	*ft_expand_inline(char *str, t_list **env_copy, _Bool had_quotes)
{
	t_ex_h	h;
	char	*new_str;
	int		j;

	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (new_str == NULL)
		return (NULL);
	new_str[0] = '\0';
	ft_init_expander_helper_nulls(&h, env_copy);
	j = 0;
	while (str[h.i])
	{
		if (str[h.i] == '$' && ft_is_var_char(str[h.i + 1]) && had_quotes)
		{
			new_str = ft_expand_variable(new_str, &h, 0, str);
			h.i = h.orig_i + h.vns;
			j = ft_strlen(new_str);
		}
		else
		{
			new_str[j++] = str[(h.i)++];
			new_str[j] = '\0';
		}
	}
	return (ft_free_for_expand_inline(new_str, str, &h));
}

void	ft_get_heredoc(t_parser **parser_node, char *delim)
{
	char	*tmp;

	tmp = NULL;
	ft_init_signals(HEREDOC_INP);
	while (1)
	{
		if ((*parser_node)->heredoc_tmp != NULL)
		{
			free((*parser_node)->heredoc);
			(*parser_node)->heredoc = ft_strdup((*parser_node)->heredoc_tmp);
			break ;
		}
		tmp = readline("> ");
		if (tmp == NULL || g_ec > 0 || ft_strcmp(delim, tmp) == 0)
			break ;
		(*parser_node)->heredoc = ft_strjoin_free((*parser_node)->heredoc, tmp);
		(*parser_node)->heredoc = \
			ft_strjoin_free((*parser_node)->heredoc, "\n");
		free(tmp);
		tmp = NULL;
	}
	ft_init_signals(NOT_INPUT);
}

int	ft_checking_for_handle_word(t_parser_h *h, t_list **env_copy)
{
	h->parser_n->cmd_args = (char **)malloc(sizeof(char *) * 2);
	if (h->parser_n->cmd_args == NULL)
		return (ft_putstr_fd(EMSG_MAL, 2), ft_free_p_node(&h->parser_n), \
			1);
	if (ft_strchr(h->lexed_i->word, '/') || (h->lexed_i->word[0] == '.' && \
		ft_strchr(h->lexed_i->word, '/')))
	{
		if (ft_handle_absolute_command(&(h->parser_n), h->lexed_i) == 1)
			return (ft_putstr_fd(EMSG_MAL, 2), \
				ft_free_p_node(&h->parser_n), 1);
	}
	else
	{
		h->parser_n->cmd_path = ft_get_path(env_copy, h->lexed_i->word, \
			&h->parser_n);
		h->parser_n->cmd_args[0] = ft_strdup(h->lexed_i->word);
	}
	if (h->parser_n->cmd_args[0] == NULL)
		return (ft_putstr_fd(EMSG_MAL, 2), 1);
	h->parser_n->cmd_args[1] = NULL;
	h->prev_was_word = 1;
	return (0);
}
