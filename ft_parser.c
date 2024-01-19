/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 18:10:12 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 18:38:10 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	ft_init_parser_helper_struct(t_parser_helper *helper)
{
	helper->error = 0;
	helper->i_cmd = -1;
	helper->p_list_head = NULL;
	helper->p_new_node = NULL;
	helper->parser_n = NULL;
	helper->prev_was_word = 0;
	helper->lexed_i = NULL;
	
}

_Bool	ft_make_command(t_parser_helper *h, int exit_code, _Bool last)
{
	if (h->error)
		return (ft_free_parser(h->p_list_head), 1);
	if (h->i_cmd != -1 || last == 1)	// if we are not at the first command or at the last command, we add the command to the list
	{
		(void)exit_code;
		/*if (ft_set_exit_err_empty_arg(&(h->parser_n), exit_code) == 1)	//only in case of "exit" command
			return (ft_free_parserh->p_list_head), 1);*/
		h->p_new_node = ft_lstnew(h->parser_n);
		if (h->p_new_node == NULL)
			return (ft_free_parser(h->p_list_head), ft_free_parser_node(&h->parser_n), perror("Malloc failed"), 1);
		ft_lstadd_back(&h->p_list_head, h->p_new_node);
	}
	if (last == 0) // if we are not at the last command, initialize a new command
	{
		h->parser_n = (t_parser *)malloc(sizeof(t_parser));
		if (h->parser_n == NULL)
			return (ft_free_parser(h->p_list_head), perror("Malloc failed"), 1);
		ft_init_parser_node(&(h->parser_n));
		h->i_cmd = h->lexed_i->exec_num;
		h->prev_was_word = 0;
	}
	return (0);
}

_Bool	ft_is_empty_lexed_lode(char *str, t_list **lexed_list, t_list *beg_of_cmd)
{

	t_lexer	*lexed_i;

	(void)str;
	(void)beg_of_cmd;
	//there is a next AND the next's exec_num is greater than the current's
	lexed_i = (t_lexer *)(*lexed_list)->content;
	if (lexed_i->wasnt_empty_var == 0)	// ''abc
		return (*lexed_list = (*lexed_list)->next, 1);	//we skip inspecting this lexed node
	return (0);	//we don't skip it
}

void	ft_parser_while(t_parser_helper *h, t_list **env_copy)
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

void	ft_free_parser_makefiles(t_list *p_list_head)
{
	t_list	*tmp;
	t_parser	*parser_node;

	tmp = p_list_head;
	while (tmp)
	{
		parser_node = (t_parser *)tmp->content;
		if (parser_node->heredoc)
		{
			free(parser_node->heredoc);
			parser_node->heredoc = NULL;
		}
		tmp = tmp->next;
	}
}

_Bool	ft_create_empty_parser_list_with_heredoc(t_parser_helper *h, t_list **ll_head, int *e_c, t_list **env)
{
	t_list	*ll_head_orig;

	ll_head_orig = *ll_head;
	while (*ll_head)
	{
		h->lexed_i = (*ll_head)->content;
		if ((h->lexed_i->exec_num > h->i_cmd) && ft_make_command(h, *e_c, 0))
			return (1);
		if (h->lexed_i->type == HEREDOC)
		{
			free(h->parser_n->heredoc_tmp);
			h->parser_n->heredoc_tmp = NULL;
			ft_handle_heredoc(&(h->parser_n), h->lexed_i, &(h->error), env);
			if (h->error || g_sig == CNTRL_C)
				return (ft_free_parser_node(&h->parser_n), 
					ft_free_parser(h->p_list_head), 1);
		}
		*ll_head = (*ll_head)->next;
	}
	if (ft_make_command(h, *e_c, 1) == 1)
		return (1);
	*ll_head = ll_head_orig;
	ft_free_parser_makefiles(h->p_list_head);
	h->i_cmd = -1;
	return (0);
}

void	ft_normie_shortener1(t_parser_helper *h, t_list **lexed_list, t_list **first_cmd_lexed)
{
	if (((t_lexer *)(*lexed_list)->content)->exec_num > h->i_cmd)
		*first_cmd_lexed = *lexed_list;
	if (h->parser_n && h->parser_n->exit_code != 0)
	{	
		while ((*lexed_list) && ((t_lexer *)(*lexed_list)->content)->exec_num == h->i_cmd)
			(*lexed_list) = (*lexed_list)->next;
	}
	h->i_cmd = h->lexed_i->exec_num;
	if (*lexed_list != NULL)
		h->lexed_i = (t_lexer *)(*lexed_list)->content;
}

void	ft_normie_shortener2(t_parser_helper *h)
{
	if ((h->lexed_i->exec_num > h->i_cmd))
	{
		h->p_new_node = h->p_new_node->next;
		h->prev_was_word = 0;
	}
}

t_list	*ft_parser(t_list *lexed_list, int *exit_c, t_list **env_copy)
{
	t_parser_helper	h;
	t_list			*tmp;
	t_list			*first_cmd_lexed;

	if (lexed_list == NULL)
		return (NULL);
	tmp = lexed_list;
	ft_init_parser_helper_struct(&h);
	if (ft_create_empty_parser_list_with_heredoc(&h, &lexed_list, exit_c, env_copy))
		return (NULL);
	h.p_new_node = h.p_list_head;
	while (lexed_list)
	{
		ft_normie_shortener1(&h, &lexed_list, &first_cmd_lexed);
		if (lexed_list == NULL)
			break ;
		if (ft_is_empty_lexed_lode(h.lexed_i->word, &lexed_list, first_cmd_lexed))
			continue ;
		ft_normie_shortener2(&h);
		ft_parser_while(&h, env_copy);
		if (h.error)
			return (ft_free_parser(h.p_list_head), NULL);
		lexed_list = lexed_list->next;
	}
	lexed_list = tmp;
	return (h.p_list_head);
}
