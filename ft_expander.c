/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 10:37:28 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 22:04:23 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	init_expander_helper(t_ex_h *h, t_list **lexed_list, \
	t_list **env_copy)
{
	h->list_head = *lexed_list;
	h->current_node = h->list_head;
	h->env_copy = env_copy;
	h->i = 0;
	h->orig_i = 0;
	h->vns = 0;
	h->var_value = NULL;
	h->needs_expansion = 0;
}

char	*ft_checker_for_expander(t_ex_h *h, int exit_code, char *str)
{
	if (str[h->i] == '\'')
		str = ft_remove_quote(str, &h->i, '\'');
	else if (str[h->i] == '\"')
		str = ft_expand_dquote(h, exit_code);
	else if (str[h->i] == '$')
		str = ft_expand_with_split(h, &exit_code);
	else
		h->i++;
	return (str);
}

t_list	*ft_expander(t_list **lexed_list, t_list **env_copy, int exit_code)
{
	t_ex_h	h;
	char	*str;

	init_expander_helper(&h, lexed_list, env_copy);
	if (!(lexed_list))
		return (NULL);
	while (h.current_node)
	{
		h.i = 0;
		str = ((t_lexer *)h.current_node->content)->word;
		while (str[h.i] && \
			((t_lexer *)h.current_node->content)->type != HEREDOC)
		{
			str = ft_checker_for_expander(&h, exit_code, str);
			((t_lexer *)h.current_node->content)->word = str;
			if (!((t_lexer *)h.current_node->content)->word)
				return (ft_free_lexer(h.list_head), h.list_head = NULL, NULL);
		}
		h.current_node = h.current_node->next;
	}
	ft_rearrange_lexed_list(&(h.list_head), -1, &h);
	return (h.list_head);
}
