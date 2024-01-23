/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_utils3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 20:36:10 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/23 11:00:19 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_expand_with_split(t_ex_h *h, int *exit_code)
{
	char	*new_str;
	t_lexer	*o_node;

	h->needs_expansion = 0;
	o_node = (t_lexer *)(h->current_node)->content;
	if (o_node->word[h->i + 1] == '\0' || o_node->word[h->i + 1] == '$' )
		return ((h->i)++, o_node->word);
	new_str = malloc(sizeof(char) * ft_strlen(o_node->word) + 1);
	if (!new_str)
		return (ft_putstr_fd(EMSG_MAL, 2), NULL);
	ft_strncpy(new_str, o_node->word, (size_t)h->i);
	new_str[h->i] = '\0';
	if (o_node->word[(h->i) + 1] == '?')
		return (ft_handle_dollar_question(new_str, exit_code, &h->i, \
			o_node->word));
	new_str = ft_expand_variable(new_str, h, &h->needs_expansion, o_node->word);
	if (!new_str)
		return (NULL);
	if (new_str[h->orig_i] == '\0' && h->orig_i == 0 && \
		o_node->word[h->vns] == '\0')
		o_node->wasnt_empty_var = 0;
	if (ft_strchr(new_str, ' ') != NULL && o_node->type != WORD && \
		o_node->type != HEREDOC)
		return (p_amb_re(ft_get_var_name(o_node->word)), *exit_code = 1, NULL);
	return (ft_checker_for_expand_with_split(h, new_str, o_node));
}

int	ft_get_number_of_nodes_in_cmd(t_list *lexed_l, int i)
{
	int		number_of_nodes_in_cmd;
	t_list	*beg;

	number_of_nodes_in_cmd = 0;
	beg = lexed_l;
	while (lexed_l && ((t_lexer *)lexed_l->content)->exec_num == i)
	{
		number_of_nodes_in_cmd++;
		lexed_l = lexed_l->next;
	}
	lexed_l = beg;
	return (number_of_nodes_in_cmd);
}

void	ft_helper_put_node_to_end_of_cmd(t_list *t_b_m, t_list **first_in_cmd, \
	t_list **orig, t_list *prev)
{
	if (t_b_m == *first_in_cmd)
	{
		*first_in_cmd = t_b_m->next;
		*orig = *first_in_cmd;
	}
	else
	{
		while (prev->next != t_b_m)
			prev = prev->next;
		prev->next = (t_b_m)->next;
		*orig = prev->next;
	}
}

void	ft_put_node_to_end_of_cmd(t_list **orig, t_list **first_in_cmd, \
t_list *t_b_m)
{
	t_lexer	*lexed_l_cont;
	t_list	*prev;

	lexed_l_cont = (t_lexer *)(t_b_m)->content;
	if (!(t_b_m)->next || ((t_lexer *)(t_b_m)->next->content)->exec_num != \
		lexed_l_cont->exec_num)
		return ;
	prev = *first_in_cmd;
	ft_helper_put_node_to_end_of_cmd(t_b_m, first_in_cmd, orig, prev);
	prev = *first_in_cmd;
	while ((prev)->next && ((t_lexer *)(prev)->next->content)->exec_num == \
		lexed_l_cont->exec_num)
		(prev) = (prev)->next;
	if (!(prev->next))
		(t_b_m)->next = NULL;
	else
		(t_b_m)->next = prev->next;
	prev->next = t_b_m;
}

void	ft_rearrange_lexed_list(t_list **lexed_l, int i, t_ex_h *h)
{
	int		max_cmds;
	int		j;
	t_list	*beg;
	int		number_of_nodes_in_cmd;

	(void)h;
	if (!lexed_l || !*lexed_l)
		return ;
	max_cmds = ((t_lexer *)ft_lstlast(*lexed_l)->content)->exec_num;
	while (++i <= max_cmds)
	{
		number_of_nodes_in_cmd = ft_get_number_of_nodes_in_cmd(*lexed_l, i);
		beg = *lexed_l;
		j = 0;
		while (lexed_l && j < number_of_nodes_in_cmd)
		{
			if (((t_lexer *)(*lexed_l)->content)->type == WORD)
				ft_put_node_to_end_of_cmd(lexed_l, &beg, *lexed_l);
			else
				*lexed_l = (*lexed_l)->next;
			j++;
		}
		*lexed_l = beg;
	}
}
