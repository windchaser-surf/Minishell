/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 13:14:16 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 19:31:38 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_skip_spaces(char *str, int *i)
{
	if (str[*i] == '\0')
		return ;
	while (*(str + *i) != '\0' && *(str + *i) == ' ')
		(*i)++;
}

void	ft_skip_to_closing_quote(char *command, int *i, char close_char)
{
	if (!command)
		return ;
	(*i)++;
	while (command[*i] != close_char)
		(*i)++;
	*i += 1;
}

void	ft_free_array(char **arr)
{
	int	i;

	i = 0;
	if (arr == NULL)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	ft_free_lexer_node(t_list *lexer_head)
{
	free(((t_lexer *)lexer_head->content)->word);
	ft_lstdelone(lexer_head, free);
}

void	ft_free_lexer(t_list *lexer_head)
{
	t_list	*tmp;

	while (lexer_head)
	{
		tmp = lexer_head->next;
		free(((t_lexer *)lexer_head->content)->word);
		ft_lstdelone(lexer_head, free);
		lexer_head = tmp;
		if (lexer_head == NULL)
			break ;
	}
}
