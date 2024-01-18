/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:49:53 by rluari            #+#    #+#             */
/*   Updated: 2024/01/18 22:19:14 by rluari           ###   ########.fr       */
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
		printf("wasnt_empty_var: %d\n", lexer_node->wasnt_empty_var);
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


_Bool	ft_handle_lexer_word(t_lexer_helper *helper, char *command, _Bool skip_spaces, int *exit_code)
{
	if (ft_make_lnode(helper, command) == 1)
		return (1);
	helper->prev_wt = WORD;
	(void)skip_spaces;
	char *asd = ((t_lexer *)helper->list_head->content)->word;
	ft_skip_spaces(command, &(helper->i));
	if (helper->prev_wt != WORD)
		if (ft_check_word_first_letter(command[helper->i], helper->list_head, exit_code) == 1)
			return (1);
	helper->start = helper->i;
	(void)asd;
	return (0);
}

_Bool	ft_handle_lexer_redir(t_lexer_helper *helper, char *command, int *exit_code)
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
		if (ft_check_word_first_letter(command[helper->i], helper->list_head, exit_code) == 1)//for this case: cat asd > >of1
			return (1);
	}
	helper->start = helper->i;
	return (0);
}

_Bool	ft_handle_lexer_input(t_lexer_helper *helper, char *command, int *exit_code)
{
	if (ft_make_lnode(helper, command) == 1)
		return (1);
	helper->prev_wt = INFILE;
	if (command[helper->i + 1] == '<')
	{
		helper->prev_wt = HEREDOC;
		helper->i++;
	}
	helper->i++;
	ft_skip_spaces(command, &(helper->i));
	if (helper->prev_wt != WORD && helper->prev_wt != HEREDOC)
	{
		if (ft_check_word_first_letter(command[helper->i], helper->list_head, exit_code) == 1)
			return (1);
	}
	helper->start = helper->i;
	return (0);
}

_Bool	ft_lexer_while(t_lexer_helper *helper, char *command, int *exit_code)
{
	if (command[helper->i] == ' ')	//a word ends with a space or a redirection sign
	{
		if (ft_handle_lexer_word(helper, command, 1, exit_code) == 1)
			return (1);
	}
	else if (command[helper->i] == '|')
	{
		if (ft_handle_lexer_new_command(helper, command, exit_code) == 1)
			return (1);
	}
	else if (command[helper->i] == '>' )	//we alreaady know that it doesnt end with a redir char
	{
		if (ft_handle_lexer_redir(helper, command, exit_code) == 1)
			return (1);
	}
	else if (command[helper->i] == '<')
	{
		if (ft_handle_lexer_input(helper, command, exit_code) == 1)
			return (1);
	}
	else if (command[helper->i] == '\'' || command[helper->i] == '\"')
		ft_skip_to_closing_quote(command, &(helper->i), command[helper->i]);
	else
		helper->i++;
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