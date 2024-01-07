/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 18:10:12 by rluari            #+#    #+#             */
/*   Updated: 2024/01/07 14:23:20 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	ft_init_parser_helper_struct(t_parser_helper *helper)
{
	helper->error = 0;
	helper->ith_command = -1;
	helper->list_head = NULL;
	helper->new_node_head = NULL;
	helper->parser_n = NULL;
}

_Bool	ft_create_new_command(t_parser_helper *h, int exit_code, _Bool last)
{
	if (h->error)
		return (ft_free_parser(h->list_head), 1);
	if (h->ith_command != -1 || last == 1)	// if we are not at the first command or at the last command, we add the command to the list
	{
		if (ft_set_exit_err_empty_arg(&(h->parser_n), exit_code) == 1)	//only in case of "exit" command
			return (ft_free_parser(h->list_head), 1);
		h->new_node_head = ft_lstnew(h->parser_n);
		if (h->new_node_head == NULL)
			return (ft_free_parser(h->list_head), perror("Malloc failed"), 1);
		ft_lstadd_back(&(h->list_head), h->new_node_head);
	}
	if (last == 0) // if we are not at the last command, initialize a new command
	{
		h->parser_n = (t_parser *)malloc(sizeof(t_parser));
		if (h->parser_n == NULL)
			return (ft_free_parser(h->list_head), perror("Malloc failed"), 1);
		ft_init_parser_node(&(h->parser_n));
		h->ith_command = h->lexed_i->exec_num;
		h->prev_was_word = 0;
	}
	return (0);
}

_Bool	ft_is_empty_lexed_lode(char *str, t_list **lexed_list, t_list *beg_of_cmd)
{
	//TODO: only true if there is nothing after, otherwise return 0
	_Bool	nothing_after;
	_Bool	nothing_before;
	t_lexer	*lexed_i;
	t_lexer	*lexed_i_next;
	
	nothing_after = 0;
	nothing_before = 0;
	//there is a next AND the next's exec_num is greater than the current's
	lexed_i = (t_lexer *)(*lexed_list)->content;
	if (!(*lexed_list)->next && beg_of_cmd != *lexed_list && str[0] == '\0')
		return (*lexed_list = (*lexed_list)->next, 1);
	else if ((*lexed_list)->next)
	{
		lexed_i_next = (t_lexer *)(*lexed_list)->next->content;
		if (lexed_i->exec_num < lexed_i_next->exec_num)
			nothing_after = 1;
	}
	else if (!(*lexed_list)->next)
		nothing_after = 1;
	if (beg_of_cmd == *lexed_list)
		nothing_before = 1;
	
	if (str[0] == '\0' && !nothing_after && !nothing_before)
		return (*lexed_list = (*lexed_list)->next, 1);
	return (0);
}

void	ft_parser_while(t_parser_helper *h, t_list **env_copy)
{
	if (h->lexed_i->type == REDIR || h->lexed_i->type == D_REDIR)
		ft_handle_redirs(&(h->parser_n), h->lexed_i, h->lexed_i->type);
	else if (h->lexed_i->type == INPUT)
		ft_handle_input(&(h->parser_n), h->lexed_i, &(h->error));
	else if (h->lexed_i->type == HEREDOC)
		ft_handle_heredoc(&(h->parser_n), h->lexed_i, &(h->error));
	else if (h->lexed_i->type == WORD)
		h->error = ft_handle_word(h, env_copy);
}

t_list	*ft_parser(t_list *lexed_list, int *exit_c, t_list **env_copy)
{
	t_parser_helper	h;
	t_list			*tmp;
	t_list			*first_cmd_lexed;

	tmp = lexed_list;
	if (lexed_list == NULL)
		return (NULL);
	ft_init_parser_helper_struct(&h);
	while (lexed_list)
	{
		
		if (((t_lexer *)lexed_list->content)->exec_num > h.ith_command)
			first_cmd_lexed = lexed_list;
		if (h.parser_n && h.parser_n->exit_code != 0)
		{	
			while (lexed_list && ((t_lexer *)lexed_list->content)->exec_num == h.ith_command)
				lexed_list = lexed_list->next;
		}
		if (lexed_list == NULL)
			break ;
		h.lexed_i = lexed_list->content;
		if (ft_is_empty_lexed_lode(h.lexed_i->word, &lexed_list, first_cmd_lexed)) // if the lexed item is empty, we skip it ONLY IF something before or after
			continue ;
		if ((h.lexed_i->exec_num > h.ith_command) && ft_create_new_command(&h, *exit_c, 0))
			return (NULL);
		ft_parser_while(&h, env_copy);
		lexed_list = lexed_list->next;
	}
	lexed_list = tmp;
	if (ft_create_new_command(&h, *exit_c, 1) == 1)
		return (NULL);
	
	return (h.list_head);
}
