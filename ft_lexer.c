/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:49:53 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 19:39:49 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void	ft_print_lexer_list(t_list *list)
{
	t_lexer	*lexer_node;

	while (list)
	{
		lexer_node = (t_lexer *)list->content;
		printf("word: %s\n", ((t_lexer *)list->content)->word);
		printf("type: %d\n", lexer_node->type);
		printf("exec_num: %d\n", lexer_node->exec_num);
		printf("wasnt_empty_var: %d\n", lexer_node->wasnt_empty_var);
		printf("\n");
		list = list->next;
	}
}*/

_Bool	ft_handle_lexer_word(t_lexer_helper *h, char *command, int *e_c)
{
	if (ft_make_lnode(h, command) == 1)
		return (1);
	h->prev_wt = WORD;
	ft_skip_spaces(command, &(h->i));
	if (h->prev_wt != WORD)
		if (ft_check_word_first_letter(command[h->i], h->list_head, e_c) == 1)
			return (1);
	h->start = h->i;
	return (0);
}

_Bool	ft_handle_lexer_redir(t_lexer_helper *h, char *command, int *e_c)
{
	if (ft_make_lnode(h, command) == 1)
		return (1);
	h->prev_wt = REDIR;
	if (command[h->i + 1] == '>')
	{
		h->prev_wt = D_REDIR;
		h->i++;
	}
	h->i++;
	ft_skip_spaces(command, &(h->i));
	if (h->prev_wt != WORD)
	{
		if (ft_check_word_first_letter(command[h->i], h->list_head, e_c) == 1)
			return (1);
	}
	h->start = h->i;
	return (0);
}

_Bool	ft_handle_lexer_input(t_lexer_helper *h, char *command, int *e_c)
{
	if (ft_make_lnode(h, command) == 1)
		return (1);
	h->prev_wt = INFILE;
	if (command[h->i + 1] == '<')
	{
		h->prev_wt = HEREDOC;
		h->i++;
	}
	h->i++;
	ft_skip_spaces(command, &(h->i));
	if (h->prev_wt != WORD && h->prev_wt != HEREDOC)
	{
		if (ft_check_word_first_letter(command[h->i], h->list_head, e_c) == 1)
			return (1);
	}
	h->start = h->i;
	return (0);
}

_Bool	ft_lexer_while(t_lexer_helper *h, char *command, int *exit_code)
{
	if (command[h->i] == ' ')
	{
		if (ft_handle_lexer_word(h, command, exit_code) == 1)
			return (1);
	}
	else if (command[h->i] == '|')
	{
		if (ft_handle_lexer_new_command(h, command, exit_code) == 1)
			return (1);
	}
	else if (command[h->i] == '>' )
	{
		if (ft_handle_lexer_redir(h, command, exit_code) == 1)
			return (1);
	}
	else if (command[h->i] == '<')
	{
		if (ft_handle_lexer_input(h, command, exit_code) == 1)
			return (1);
	}
	else if (command[h->i] == '\'' || command[h->i] == '\"')
		ft_skip_to_closing_quote(command, &(h->i), command[h->i]);
	else
		h->i++;
	return (0);
}

t_list	*ft_lexer(char *command, int *exit_code)
{
	t_lexer_helper	helper;

	ft_init_lexer_helper(&helper, command, 0);
	while (command[helper.i])
	{
		if (ft_lexer_while(&helper, command, exit_code) == 1)
			return (NULL);
	}
	if (helper.start != helper.i)
	{
		if (ft_make_lnode(&helper, command))
			return (NULL);
	}
	return (helper.list_head);
}
