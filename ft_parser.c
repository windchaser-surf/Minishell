/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 18:10:12 by rluari            #+#    #+#             */
/*   Updated: 2023/12/13 17:06:30 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_parser(t_list *lexed_list)
{
	t_list		*list_head;
	t_list		*new;
	t_list		*out_list_head;

	t_parser	*parser_node;
	t_lexer		*lexed_item;
	int			ith_command;
	_Bool		error;
	_Bool		prev_was_word;

	error = 0;
	ith_command = -1;
	list_head = NULL;
	new = NULL;
	parser_node = NULL;
	out_list_head = NULL;
	if (lexed_list == NULL)
		return (NULL);
	while (lexed_list)
	{
		lexed_item = lexed_list->content;
		if (lexed_item->exec_num > ith_command)
		{
			if (ith_command != -1)
			{
				new = ft_lstnew(parser_node);
				if (new == NULL)
					return (ft_free_parser(list_head) ,NULL);
				ft_lstadd_back(&list_head, new);
			}
			parser_node = (t_parser *)malloc(sizeof(t_parser));
			if (parser_node == NULL)
				return (ft_free_parser(list_head), NULL);
			ft_init_parser_node(&parser_node);
			ith_command = lexed_item->exec_num;
			prev_was_word = 0;
		}
		if (lexed_item->type == REDIRECTION)
			ft_handle_redirs(&parser_node, lexed_item, &error, lexed_item->type);
		else if (lexed_item->type == DOUBLE_REDIRECTION)
			ft_handle_redirs(&parser_node, lexed_item, &error, lexed_item->type);
		else if (lexed_item->type == INPUT)
			ft_handle_input(&parser_node, lexed_item, &error);
		else if (lexed_item->type == HEREDOC)
			ft_handle_heredoc(&parser_node, lexed_item, &error);
		else if (lexed_item->type == WORD)
			ft_handle_word(&parser_node, lexed_item, &error, &prev_was_word);
		else
			printf("Error: unknown type\n");
		if (error)
			return (ft_free_parser(list_head), NULL);
		lexed_list = lexed_list->next;
	}
	new = ft_lstnew(parser_node);
	if (new == NULL)
		return (ft_free_parser(list_head) ,NULL);
	ft_lstadd_back(&list_head, ft_lstnew(new));
	
	return (list_head);
}