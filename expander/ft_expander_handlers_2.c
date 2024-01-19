/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_handlers_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 20:01:36 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 22:08:50 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	p_amb_re(char *var_name)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(var_name, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
}

char	*ft_get_var_name(char *str)
{
	char	*res;
	int		j;

	j = 0;
	res = malloc(sizeof(char) * ft_strlen(&str[j]) + 1);
	if (!res)
		return (ft_putstr_fd(EMSG_MAL, 2), NULL);
	res[j] = '$';
	j++;
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
	{
		res[j] = str[j];
		j++;
	}
	res[j] = '\0';
	return (res);
}

int	ft_get_var_name_size(char *str, int *i)
{
	int	vns;

	*i += 1;
	vns = 0;
	while (str[*i + vns] && ft_is_var_char(str[*i + vns]))
		(vns)++;
	return (vns);
}

char	*ft_insert_new_lexed_nodes(t_list *new_nodes_head, t_ex_h *h)
{
	t_list	*new_current;
	t_list	*tmp;

	new_current = ft_lstlast(new_nodes_head);
	tmp = h->list_head;
	if (tmp == (h->current_node))
		h->list_head = new_nodes_head;
	else
	{
		while (tmp->next != (h->current_node) && tmp->next)
			tmp = tmp->next;
		tmp->next = new_nodes_head;
	}
	if ((h->current_node)->next != NULL)
		new_current->next = (h->current_node)->next;
	ft_free_lexer_node(h->current_node);
	h->current_node = new_current;
	return (((t_lexer *)h->current_node->content)->word);
}
