/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:49:53 by rluari            #+#    #+#             */
/*   Updated: 2023/12/12 16:13:33 by codespace        ###   ########.fr       */
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
		printf("word: %s\n", lexer_node->word);
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

t_list	*ft_lexer(char *command)
{
	t_lexer	*lexer_node;
	t_list	*list_head;
	t_list	*new;
	int		i;
	int		start;
	int		exec_num;

	i = 0;
	exec_num = 0;
	WordTyp	prev_wt = WORD;
	list_head = NULL;
	lexer_node = NULL;
	new = NULL;
	ft_skip_spaces(command + i, &i);
	start = i;
	if (command[i] == '|')
	{
		ft_putstr_fd("Minishell: syntax error near unexpected token `|'\n", 2);
		return (NULL);
	}
	while (command[i])
	{
		if (command[i] == ' ' && command[i - 1] != '\'' && command[i - 1] != '\"')
		{
			lexer_node = (t_lexer *)malloc(sizeof(t_lexer));
			if (lexer_node == NULL)
				return (NULL);
			
			lexer_node->word = ft_substr(command, start, i - start);
			lexer_node->type = prev_wt;
			lexer_node->exec_num = exec_num;
			new = ft_lstnew(lexer_node);
			if (new == NULL)
				return (NULL);
			ft_lstadd_back(&list_head, new);
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
			exec_num++;
			i++;
			ft_skip_spaces(command + i, &i);
			if (prev_wt != WORD)
			{
				if (ft_check_word_first_letter(command[i], list_head) == 1)
					return (NULL);
			}
			start = i;
			//i++;
			
		}
		else if (command[i] == '>')
		{
			//if it turns out that you have to check what's before the FtRedir operator
			//based on this site:
			// https://www.gnu.org/software/bash/manual/bash.html#Redirecting-Input
			//then here we have to implement a function that goes backward and checks the number
			//which will be the fd
			prev_wt = REDIRECTION;
			if ((command + i + 1) && command[i + 1] == '>')
			{
				prev_wt = DOUBLE_REDIRECTION;
				i++;
			}
			i++;
			ft_skip_spaces(command + i, &i);
			if (prev_wt != WORD)
			{
				if (ft_check_word_first_letter(command[i], list_head) == 1)
					return (NULL);
			}
			start = i;
			//i++;
		}
		else if ((command + i + 1) && command[i] == '<')
		{
			prev_wt = INPUT;
			if (command[i + 1] == '<')
			{
				prev_wt = HEREDOC;
				i++;
			}
			i++;
			ft_skip_spaces(command + i, &i);
			if (prev_wt != WORD)
			{
				if (ft_check_word_first_letter(command[i], list_head) == 1)
					return (NULL);
			}
			start = i;
			//i++;
		}
		else if (command[i] == '\'' || command[i] == '\"')
		{
			ft_skip_to_closing_quote(command, &i, command[i]);
			lexer_node = (t_lexer *)malloc(sizeof(t_lexer));
			if (lexer_node == NULL)
				return (NULL);
			lexer_node->word = ft_substr(command, start, i - start + 1 );	//to add also the " or '
			lexer_node->type = prev_wt;
			lexer_node->exec_num = exec_num;
			new = ft_lstnew(lexer_node);
			if (new == NULL)
				return (NULL);
			ft_lstadd_back(&list_head, new);
			prev_wt = WORD;
			i++;
			if (command[i])
				ft_skip_spaces(command + i + 1, &i);
			if (prev_wt != WORD)
			{
				if (ft_check_word_first_letter(command[i], list_head) == 1)
					return (NULL);
			}
			start = i;
		}
		else
			i++;
	}
	if (command[i - 1] != ' ' && command[i - 1] != '\'' && command[i - 1] != '\"')
	{
		lexer_node = (t_lexer *)malloc(sizeof(t_lexer));
		if (lexer_node == NULL)
			return (NULL);
		lexer_node->word = ft_substr(command, start, i - start);
		lexer_node->type = prev_wt;
		lexer_node->exec_num = exec_num;
		new = ft_lstnew(lexer_node);
		if (new == NULL)
			return (NULL);
		ft_lstadd_back(&list_head, new);
	}
	if (ft_check_for_empty_command(list_head) == 1)
		return (ft_putstr_fd("Minishell: syntax error near unexpected token `|'", 2), ft_free_lexer(list_head), NULL);
	//return (ft_free_lexer(list_head), free(command), NULL);

	return (list_head);
}