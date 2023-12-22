/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 18:10:12 by rluari            #+#    #+#             */
/*   Updated: 2023/12/22 21:13:42 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_parser_helper_struct(t_parser_helper *helper)
{
	helper->error = 0;
	helper->ith_command = -1;
	helper->list_head = NULL;
	helper->new = NULL;
	helper->parser_node = NULL;
}

_Bool	ft_create_new_command(t_parser_helper *helper, int exit_code, _Bool last)
{
	if (helper->error)
		return (ft_free_parser(helper->list_head), 1);
	if (helper->ith_command != -1)
	{
		if (ft_set_exit_error_code_empty_arg(&(helper->parser_node), exit_code) == 1)
			return (ft_free_parser(helper->list_head), 1);
		helper->new = ft_lstnew(helper->parser_node);
		if (helper->new == NULL)
			return (ft_free_parser(helper->list_head), perror("Malloc failed"), 1);
		ft_lstadd_back(&(helper->list_head), helper->new);
	}
	if (last == 0)
	{
		helper->parser_node = (t_parser *)malloc(sizeof(t_parser));
		if (helper->parser_node == NULL)
			return (ft_free_parser(helper->list_head), perror("Malloc failed"), 1);
		ft_init_parser_node(&(helper->parser_node));
		helper->ith_command = helper->lexed_item->exec_num;
		helper->prev_was_word = 0;
	}
	return (0);
}

_Bool	ft_is_empty_lexed_lode(char *str, t_list *lexed_list)
{
	if (str[0] == '\0')
		return (lexed_list = lexed_list->next, 1);
	return (0);
}

t_list	*ft_parser(t_list *lexed_list, int *exit_code, t_list **env_copy)
{
	t_parser_helper	helper;

	ft_init_parser_helper_struct(&helper);
	if (lexed_list == NULL)
		return (NULL);
	while (lexed_list)
	{
		helper.lexed_item = lexed_list->content;
		if (ft_is_empty_lexed_lode(helper.lexed_item->word, lexed_list))
			continue ;
		if (helper.lexed_item->exec_num > helper.ith_command)
		{
			if (ft_create_new_command(&helper, *exit_code, 0) == 1)
				return (NULL);
		}
		if (helper.lexed_item->type == REDIRECTION || helper.lexed_item->type == DOUBLE_REDIRECTION)
			ft_handle_redirs(&(helper.parser_node), helper.lexed_item, helper.lexed_item->type);
		else if (helper.lexed_item->type == INPUT)
			ft_handle_input(&(helper.parser_node), helper.lexed_item, &(helper.error));
		else if (helper.lexed_item->type == HEREDOC)
			ft_handle_heredoc(&(helper.parser_node), helper.lexed_item, &(helper.error));
		else if (helper.lexed_item->type == WORD)
			helper.error = ft_handle_word(&helper, env_copy);
		lexed_list = lexed_list->next;
	}
	if (ft_create_new_command(&helper, *exit_code, 0) == 1)
		return (NULL);
	return (helper.list_head);
}