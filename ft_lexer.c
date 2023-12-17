/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:49:53 by rluari            #+#    #+#             */
/*   Updated: 2023/12/15 20:20:24 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <ctype.h>

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

int	ft_check_for_empty_command(t_list *list_head)
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
}

void	ft_make_lnode(int start, int i, WordTyp prev_wt, int exec_num, t_list **list_head, char *command)
{
	t_lexer	*lexer_node;
	t_list	*new;

	if (start == i)
		return ;
	lexer_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (lexer_node == NULL)
		return ;
	lexer_node->word = ft_substr(command, start, i - start);
	lexer_node->type = prev_wt;
	lexer_node->exec_num = exec_num;
	new = ft_lstnew(lexer_node);
	if (new == NULL)
		return ;
	ft_lstadd_back(list_head, new);
}

t_list	*ft_lexer(char *command)
{
	t_list	*list_head;
	int		i;
	int		start;
	int		exec_num;

	i = 0;
	exec_num = 0;
	WordTyp	prev_wt = WORD;
	list_head = NULL;
	ft_skip_spaces(command + i, &i);
	start = i;
	if (command[i] == '|')
	{
		ft_putstr_fd("Minishell: syntax error near unexpected token `|'\n", 2);
		return (NULL);
	}
	while (command[i])
	{
		if (command[i] == ' ')	//a word ends with a space or a redirection sign
		{
			ft_make_lnode(start, i, prev_wt, exec_num, &list_head, command);
			prev_wt = WORD;
			ft_skip_spaces(command + i, &i);
			if (prev_wt != WORD)
			{
				if (ft_check_word_first_letter(command[i], list_head) == 1)
					return (NULL);
			}
			start = i;
		}
		else if (command[i] == '|')
		{
			ft_make_lnode(start, i, prev_wt, exec_num, &list_head, command);
			exec_num++;
			i++;
			ft_skip_spaces(command + i, &i);
			if (!(isalnum(command[i]) || ft_is_redirsign(command[i]))) //for this case: cat asd | | >of1
			{
				ft_putstr_fd("Error: syntax error here:", 2);
				ft_putchar_fd(command[i], 2);
				ft_putstr_fd("'\n", 2);
				ft_free_lexer(list_head);
				return (NULL);
			}
			start = i;
			prev_wt = WORD;
		}
		else if (command[i] == '>' )	//we alreaady know that it doesnt end with a redir char
		{
			ft_make_lnode(start, i, prev_wt, exec_num, &list_head, command);
			
			prev_wt = REDIRECTION;
			if (command[i + 1] == '>')
			{
				prev_wt = DOUBLE_REDIRECTION;
				i++;
			}
			i++;
			ft_skip_spaces(command + i, &i);
			
			if (prev_wt != WORD)
			{
				if (ft_check_word_first_letter(command[i], list_head) == 1)//for this case: cat asd > >of1
					return (NULL);
			}
			start = i;
		}
		else if (command[i] == '<')
		{
			ft_make_lnode(start, i, prev_wt, exec_num, &list_head, command);

			prev_wt = INPUT;
			if (command[i + 1] == '<')
			{
				prev_wt = HEREDOC;
				i++;
			}
			i++;
			ft_skip_spaces(command + i, &i);
			if (prev_wt != WORD && prev_wt != HEREDOC)
			{
				if (ft_check_word_first_letter(command[i], list_head) == 1)
					return (NULL);
			}
			start = i;
		}
		else if (command[i] == '\'' || command[i] == '\"')
		{
			ft_skip_to_closing_quote(command, &i, command[i]);
			i++;
		}
		else
			i++;
	}
	if (start != i)
	{
		ft_make_lnode(start, i, prev_wt, exec_num, &list_head, command);
	}
	if (ft_check_for_empty_command(list_head) == 1)
		return (ft_putstr_fd("Minishell: syntax error near unexpected token `|'", 2), ft_free_lexer(list_head), NULL);
	
	return (list_head);
}