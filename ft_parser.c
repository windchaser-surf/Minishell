/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 18:10:12 by rluari            #+#    #+#             */
/*   Updated: 2024/01/18 12:50:32 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	ft_init_parser_helper_struct(t_parser_helper *helper)
{
	helper->error = 0;
	helper->ith_command = -1;
	helper->p_list_head = NULL;
	helper->p_new_node = NULL;
	helper->parser_n = NULL;
	helper->prev_was_word = 0;
	helper->lexed_i = NULL;
	
}

_Bool	ft_create_new_command(t_parser_helper *h, int exit_code, _Bool last)
{
	if (h->error)
		return (ft_free_parser(h->p_list_head), 1);
	if (h->ith_command != -1 || last == 1)	// if we are not at the first command or at the last command, we add the command to the list
	{
		(void)exit_code;
		/*if (ft_set_exit_err_empty_arg(&(h->parser_n), exit_code) == 1)	//only in case of "exit" command
			return (ft_free_parser(h->p_list_head), 1);*/
		h->p_new_node = ft_lstnew(h->parser_n);
		if (h->p_new_node == NULL)
			return (ft_free_parser(h->p_list_head), ft_free_parser_node(h->parser_n), perror("Malloc failed"), 1);
		ft_lstadd_back(&(h->p_list_head), h->p_new_node);
	}
	if (last == 0) // if we are not at the last command, initialize a new command
	{
		h->parser_n = (t_parser *)malloc(sizeof(t_parser));
		if (h->parser_n == NULL)
			return (ft_free_parser(h->p_list_head), perror("Malloc failed"), 1);
		ft_init_parser_node(&(h->parser_n));
		h->ith_command = h->lexed_i->exec_num;
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
	/*else if (h->lexed_i->type == HEREDOC)
		ft_handle_heredoc(&(h->parser_n), h->lexed_i, &(h->error), env_copy);*/
	else if (h->lexed_i->type == WORD)
		h->error = ft_handle_word(h, env_copy);
}

_Bool	ft_create_empty_parser_list_with_heredoc(t_parser_helper *h, t_list **ll_head, int *exit_c, t_list **env_copy)
{
	t_list	*ll_head_orig;

	ll_head_orig = *ll_head;
	while (*ll_head)
	{
		h->lexed_i = (*ll_head)->content;
		//init empty parser node
		if ((h->lexed_i->exec_num > h->ith_command) && ft_create_new_command(h, *exit_c, 0))
			return (1);
		if (h->lexed_i->type == HEREDOC)
		{
			ft_handle_heredoc(&(h->parser_n), h->lexed_i, &(h->error), env_copy);
			if (h->error || g_sig == CNTRL_C)
				return (ft_free_parser_node(h->parser_n), ft_free_parser(h->p_list_head), 1);
		}
		*ll_head = (*ll_head)->next;
	}
	if (ft_create_new_command(h, *exit_c, 1) == 1)
		return (1);
	*ll_head = ll_head_orig;
	//ft_print_parser_list(&(h->p_list_head));
	return (0);
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
		if (((t_lexer *)lexed_list->content)->exec_num > h.ith_command)
			first_cmd_lexed = lexed_list;	//set the first lexed node of the given command
		if (h.parser_n && h.parser_n->exit_code != 0)	//if error, skip to the next command
		{	
			while (lexed_list && ((t_lexer *)lexed_list->content)->exec_num == h.ith_command)
				lexed_list = lexed_list->next;
		}
		if (lexed_list == NULL)
			break ;
		h.ith_command = h.lexed_i->exec_num;
		h.lexed_i = lexed_list->content;
		if (ft_is_empty_lexed_lode(h.lexed_i->word, &lexed_list, first_cmd_lexed)) // if the lexed item is empty, we skip it ONLY IF something before or after
			continue ;
		if ((h.lexed_i->exec_num > h.ith_command) /*&& ft_create_new_command(&h, *exit_c, 0)*/)
		{
			h.p_new_node = h.p_new_node->next;
			h.prev_was_word = 0;
		}
		ft_parser_while(&h, env_copy);
		if (h.error)
			return (ft_free_parser(h.p_list_head), NULL);
		lexed_list = lexed_list->next;
	}
	lexed_list = tmp;
	return (h.p_list_head);
}
