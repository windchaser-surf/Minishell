/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 18:10:12 by rluari            #+#    #+#             */
/*   Updated: 2024/01/24 19:09:36 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_parser_while(t_parser_h *h, t_list **env_copy)
{
	h->parser_n = (t_parser *)h->p_new_node->content;
	if (h->lexed_i->type == REDIR || h->lexed_i->type == D_REDIR)
		ft_handle_redirs(&(h->parser_n), h->lexed_i, h->lexed_i->type);
	else if (h->lexed_i->type == INFILE)
		ft_handle_input(&(h->parser_n), h->lexed_i, &(h->error));
	else if (h->lexed_i->type == HEREDOC)
		ft_handle_heredoc(&(h->parser_n), h->lexed_i, &(h->error), env_copy);
	else if (h->lexed_i->type == WORD)
		h->error = ft_handle_word(h, env_copy);
}

_Bool	ft_make_parser_list(t_parser_h *h, t_list **ll_head, t_list **env)
{
	t_list	*ll_head_orig;

	ll_head_orig = *ll_head;
	while (*ll_head)
	{
		h->lexed_i = (*ll_head)->content;
		if ((h->lexed_i->exec_num > h->i_cmd) && ft_make_command(h, 0))
			return (1);
		if (h->lexed_i->type == HEREDOC)
		{
			free(h->parser_n->heredoc_tmp);
			h->parser_n->heredoc_tmp = NULL;
			ft_handle_heredoc(&(h->parser_n), h->lexed_i, &(h->error), env);
			if (h->error || g_ec > 0)
				return (ft_free_p_node(&h->parser_n), free(h->parser_n),
					ft_free_parser(h->p_list_head), 1);
		}
		*ll_head = (*ll_head)->next;
	}
	if (ft_make_command(h, 1) == 1)
		return (1);
	*ll_head = ll_head_orig;
	ft_free_parser_makefiles(h->p_list_head);
	h->i_cmd = -1;
	return (0);
}

void	ft_normie_shortener1(t_parser_h *h, t_list **lexed_l, t_list **f_cmd_l)
{
	if (((t_lexer *)(*lexed_l)->content)->exec_num > h->i_cmd)
		*f_cmd_l = *lexed_l;
	if (h->parser_n && h->parser_n->exit_code != 0)
	{
		while ((*lexed_l)
			&& ((t_lexer *)(*lexed_l)->content)->exec_num == h->i_cmd)
			(*lexed_l) = (*lexed_l)->next;
	}
	h->i_cmd = h->lexed_i->exec_num;
	if (*lexed_l != NULL)
		h->lexed_i = (t_lexer *)(*lexed_l)->content;
}

void	ft_normie_shortener2(t_parser_h *h, t_list **env_copy)
{
	if ((h->lexed_i->exec_num > h->i_cmd))
	{
		h->p_new_node = h->p_new_node->next;
		h->prev_was_word = 0;
	}
	ft_parser_while(h, env_copy);
}

t_list	*ft_parser(t_list *lexed_list, t_list **env_copy)
{
	t_parser_h	h;
	t_list		*tmp;
	t_list		*first_cmd_l;

	if (lexed_list == NULL)
		return (NULL);
	tmp = lexed_list;
	ft_init_parser_helper_struct(&h);
	if (ft_make_parser_list(&h, &lexed_list, env_copy))
		return (NULL);
	h.p_new_node = h.p_list_head;
	while (lexed_list)
	{
		ft_normie_shortener1(&h, &lexed_list, &first_cmd_l);
		if (lexed_list == NULL)
			break ;
		if (ft_is_empty_lexed_lode(&lexed_list))
			continue ;
		ft_normie_shortener2(&h, env_copy);
		if (h.error)
			return (ft_free_parser(h.p_list_head), NULL);
		lexed_list = lexed_list->next;
	}
	lexed_list = tmp;
	return (h.p_list_head);
}
