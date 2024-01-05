/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:49:53 by rluari            #+#    #+#             */
/*   Updated: 2024/01/05 11:28:46 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	ft_print_lexer_list(t_list *list)
{
	t_lexer	*lexer_node;

	while (list)
	{
		lexer_node = (t_lexer *)list->content;
		printf("word: %s\n", ((t_lexer *)list->content)->word);
		printf("type: %d\n", lexer_node->type);
		printf("exec_num: %d\n", lexer_node->exec_num);
		printf("\n");
		list = list->next;
	}
}

/*int	ft_check_for_empty_command(t_list *list_head)
{
	t_lexer	*lexer_node;

	while (list_head->next)
	{
		lexer_node = (t_lexer *)list_head->content;
		if (lexer_node->word == NULL)
		{
			return (ft_putstr_fd("Minishell: syntax error near unexpected token `|'\n", 2), 1);
		}
		list_head = list_head->next;
	}
	lexer_node = (t_lexer *)list_head->content;
	if (lexer_node->word == NULL)
	{
		return (ft_putstr_fd("Minishell: syntax error near unexpected token `|'\n", 2), 1);
	}

	
	return (0);
}*/

_Bool	ft_make_lnode(t_lexer_helper *helper, char *command)
{
	t_lexer	*lexer_node;
	t_list	*new;

	if (helper->start == helper->i)
		return (0);
	lexer_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (lexer_node == NULL)
		return (ft_free_lexer(helper->list_head), perror("Malloc failed"), 1);
	lexer_node->word = ft_substr(command, helper->start, helper->i - helper->start);
	if (lexer_node->word == NULL)
		return (ft_free_lexer(helper->list_head), perror("Malloc failed"), 1);
	lexer_node->type = helper->prev_wt;
	lexer_node->exec_num = helper->exec_num;
	new = ft_lstnew(lexer_node);
	if (new == NULL)
		return (ft_free_lexer(helper->list_head), perror("Malloc failed"), 1);
	ft_lstadd_back(&(helper->list_head), new);
	return (0);
}

void	ft_init_lexer_helper(t_lexer_helper *helper, char *command)
{
	helper->i = 0;
	helper->exec_num = 0;
	helper->prev_wt = WORD;
	helper->list_head = NULL;
	ft_skip_spaces(command + helper->i, &(helper->i));
	helper->start = helper->i;
}

_Bool	ft_handle_lexer_word(t_lexer_helper *helper, char *command)
{
	if (ft_make_lnode(helper, command) == 1)
		return (1);
	helper->prev_wt = WORD;
	ft_skip_spaces(command, &(helper->i));
	if (helper->prev_wt != WORD)
		if (ft_check_word_first_letter(command[helper->i], helper->list_head) == 1)
			return (1);
	helper->start = helper->i;
	return (0);
}

_Bool	ft_handle_lexer_new_command(t_lexer_helper *helper, char *command)
{
	if (ft_make_lnode(helper, command) == 1)
		return (1);
	helper->exec_num++;
	helper->i++;
	ft_skip_spaces(command, &(helper->i));
	if (!(isalnum(command[helper->i]) || ft_is_redirsign(command[helper->i]))) //for this case: cat asd | | >of1
		return (ft_putstr_fd("Minishell: syntax error near unexpected token `|'\n", 2), ft_free_lexer(helper->list_head), 1);
	helper->start = helper->i;
	helper->prev_wt = WORD;
	return (0);
}

_Bool	ft_handle_lexer_redir(t_lexer_helper *helper, char *command)
{
	if (ft_make_lnode(helper, command) == 1)
		return (1);
			
	helper->prev_wt = REDIR;
	if (command[helper->i + 1] == '>')
	{
		helper->prev_wt = D_REDIR;
		helper->i++;
	}
	helper->i++;
	ft_skip_spaces(command, &(helper->i));
	
	if (helper->prev_wt != WORD)
	{
		if (ft_check_word_first_letter(command[helper->i], helper->list_head) == 1)//for this case: cat asd > >of1
			return (NULL);
	}
	helper->start = helper->i;
	return (0);
}

_Bool	ft_handle_lexer_input(t_lexer_helper *helper, char *command)
{
	if (ft_make_lnode(helper, command) == 1)
		return (1);
	helper->prev_wt = INPUT;
	if (command[helper->i + 1] == '<')
	{
		helper->prev_wt = HEREDOC;
		helper->i++;
	}
	helper->i++;
	ft_skip_spaces(command, &(helper->i));
	if (helper->prev_wt != WORD && helper->prev_wt != HEREDOC)
	{
		if (ft_check_word_first_letter(command[helper->i], helper->list_head) == 1)
			return (1);
	}
	helper->start = helper->i;
	return (0);
}

_Bool	ft_lexer_while(t_lexer_helper *helper, char *command)
{
	if (command[helper->i] == ' ')	//a word ends with a space or a redirection sign
	{
		if (ft_handle_lexer_word(helper, command) == 1)
			return (1);
	}
	else if (command[helper->i] == '|')
	{
		if (ft_handle_lexer_new_command(helper, command) == 1)
			return (1);
	}
	else if (command[helper->i] == '>' )	//we alreaady know that it doesnt end with a redir char
	{
		if (ft_handle_lexer_redir(helper, command) == 1)
			return (1);
	}
	else if (command[helper->i] == '<')
	{
		if (ft_handle_lexer_input(helper, command) == 1)
			return (1);
	}
	else if (command[helper->i] == '\'' || command[helper->i] == '\"')
	{
		ft_skip_to_closing_quote(command, &(helper->i), command[helper->i]);
		helper->i++;
	}
	else
		helper->i++;
	return (0);
}

t_list	*ft_lexer(char *command)
{
	t_lexer_helper	helper;

	ft_init_lexer_helper(&helper, command);
	while (command[helper.i])
	{
		if (ft_lexer_while(&helper, command) == 1)
			return (NULL);
	}
	if (helper.start != helper.i)
	{
		if (ft_make_lnode(&helper, command))
			return (NULL);
	}
	return (helper.list_head);
}