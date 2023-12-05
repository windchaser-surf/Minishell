/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 14:10:26 by rluari            #+#    #+#             */
/*   Updated: 2023/12/04 17:46:05 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int	ft_check_word_first_letter(char c, t_list *lexer_head)
{
	if (ft_isalnum(c) == 0 && c != ' ')
	{
		ft_putstr_fd("Error: syntax error here:", 2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("'\n", 2);
		ft_free_lexer(lexer_head);
		return (1);
	}
	return (0);	//no error
}

void	ft_skip_spaces(char *str, int *i)
{
	int	j;

	j = 0;
	while (str + j && str[j] == ' ')
		j++;
	(*i) += j;
}

void	ft_skip_to_closing_quote(char *command, int *i, char close_char)
{
	while (command[*i] != close_char)
		(*i)++;
}

void	ft_free_lexer(t_list *lexer_head)
{
	t_list	*tmp;

	while (lexer_head)
	{
		tmp = lexer_head->next;
		free(((t_lexer *)lexer_head->content)->word);
		free(lexer_head->content);
		free(lexer_head);
		lexer_head = tmp;
		if (lexer_head == NULL)
			break ;
	}
}