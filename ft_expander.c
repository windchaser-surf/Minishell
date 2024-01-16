/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 10:37:28 by rluari            #+#    #+#             */
/*   Updated: 2024/01/16 13:00:01 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void remove_node(t_list **head, t_list *node_to_remove) {
    // Check if the list is empty
    if (*head == NULL || node_to_remove == NULL) {
        return;
    }

    // Check if the node to be removed is at the head of the list
    if (*head == node_to_remove) {
        *head = (*head)->next;
        free(node_to_remove);
        return;
    }

    // Traverse the list to find the node before the one to be removed
    t_list *current = *head;
    while (current != NULL && current->next != node_to_remove) {
        current = current->next;
    }

    // Check if the node before the one to be removed is found
    if (current != NULL) {
        current->next = node_to_remove->next;
        free(node_to_remove);
    }
}

char	*ft_remove_quote(char *str, int *i, char c)	//from ab"cde"fg"hi" to abcdefg"hi", and setting i to the position of f
{
	char	*tmp;
	int		j;

	tmp = malloc(sizeof(char) * ft_strlen(str) - 2 + 1);
	if (!tmp)
		return (NULL);
	ft_strncpy(tmp, str, (size_t)*i);	//copy everything before the quote
	(*i)++;	//skip the quote
	while (str[*i] && str[*i] != c)	//copy everything between the quotes
	{
		tmp[*i - 1] = str[*i];
		*i += 1;
	}
	(*i)++;	//skip the quote
	j = 0;
	while (str[*i + j])	//copy everything after the quote
	{
		tmp[*i + j - 2] = str[*i + j];
		j++;
	}
	tmp[*i + j - 2] = '\0';
	*i = *i - 2;
	free(str);
	return (tmp);
}

char	*ft_make_new_str_for_expanstion(int *i, char *str, int *j)
{
	char	*new_str;

	new_str = malloc(sizeof(char) * ft_strlen(str) - 2 + 1);
	if(!(new_str))
		return (perror("Malloc failed"), NULL);
	ft_strncpy(new_str, str, (size_t)*i);	//copy everything before the quote
	new_str[*i] = '\0';
	(*i)++;	//skip the quote
	*j = *i - 1;
	return (new_str);
}

void	ft_copy_whats_after_the_quote(int *i, int *j, char *str, char *new_str)
{
	(*i)++;	//skip the closing quote
	int tmp_i = *i;
	*i = *j;
	while (str[tmp_i])	//copy everything after the quote
		new_str[(*j)++] = str[tmp_i++];
	new_str[*j] = '\0';
	free(str);
}

char	*ft_expand_dquote(t_expander_helper *h, int exit_code)	//from ab"cde"fg"hi" to abcdefghi, and setting i to the position of f
{
	char	*new_str;
	int		j;
	char	*str;
	str = ((t_lexer *)h->current_node->content)->word;
	new_str = ft_make_new_str_for_expanstion(&h->i, str, &j);
	if (!new_str)
		return (NULL);
	while (str[h->i] && str[h->i] != '\"')	//copy everything between the quotes
	{
		if (str[h->i] == '$' && (ft_isalpha(str[(h->i) + 1])
			|| str[(h->i) + 1] == '?' || str[(h->i) + 1] == '_'))	//expand variable if it is not the last char or another $
		{
			if (str[(h->i) + 1] == '?')
				new_str = ft_handle_dollar_question_q(new_str, &exit_code, &h->i, str);
			else
				new_str = ft_expand_variable(new_str, h, NULL);	//it expands and attaches to the end of the string
			j = ft_strlen(new_str);
		}
		else
		{
			new_str[j++] = str[(h->i)++];
			new_str[j] = '\0';
		}
	}
	ft_copy_whats_after_the_quote(&h->i, &j, str, new_str);
	return (new_str);
}

t_list	*ft_lexer_but_with_words_and_one_cmd(char *command, int cmd_num)
{
	t_lexer_helper	helper;

	ft_init_lexer_helper(&helper, command, cmd_num);
	//ft_skip_spaces(command, &helper.i);
	while (command[helper.i])
	{
		if (command[helper.i] == ' ')	//a word ends with a space or a redirection sign
		{
			if (ft_handle_lexer_word(&helper, command) == 1)
				return (NULL);
		}
		else
			helper.i++;
	}
	if (helper.start != helper.i)
	{
		if (ft_make_lnode(&helper, command))
			return (NULL);
	}
	
	return (helper.list_head);
}

char	*ft_concat_rest(char *str, t_expander_helper *h, char *new_str, _Bool needs_expansion)
{
	char	*rest;

	if (needs_expansion)
		h->i = ft_strlen(new_str);
	rest = ft_substr(str, h->orig_i + h->vns, ft_strlen(str) - (h->orig_i + h->vns));
	if (!rest)
		return (perror("Malloc failed"), NULL);
	new_str = ft_strjoin_free(new_str, rest);
	free(rest);
	//h->i = h->i - h->vns + ft_strlen(h->var_value) - 1;
	free(h->var_value);
	return (new_str);
}

char	*ft_attach_beginning(char *head_node_str, char *str, t_expander_helper *h)
{
	char	*tmp;

	tmp = malloc(sizeof(char) * ft_strlen(head_node_str) + h->orig_i - 1 + 1);
	if (!tmp)
		return (perror("Malloc failed"), NULL);
	ft_strlcpy(tmp, str, (size_t)h->orig_i);
	//ft_strncpy(tmp, str, (size_t)h->orig_i - 1);
	ft_strlcat(tmp, head_node_str, ft_strlen(head_node_str) + h->orig_i - 1 + 1);
	free(head_node_str);
	return (tmp);
	
}

char	*ft_expand_with_split(t_expander_helper *h, int *exit_code)
{
	char	*new_str;
	t_lexer *orig_lex_node;
	t_list	*new_nodes_head;
	_Bool	needs_expansion;

	needs_expansion = 0;
	orig_lex_node  = (t_lexer *)(h->current_node)->content;	//we are in this one, only in this
	if (orig_lex_node->word[h->i + 1] == '\0' || orig_lex_node->word[h->i + 1] == '$')	//if it is the last char or another $, then it is not a variable, so we return
		return ((h->i)++, orig_lex_node->word);
	new_str = malloc(sizeof(char) * ft_strlen(orig_lex_node->word) + 1);
	if (!new_str)
		return (perror("Malloc failed"), NULL);
	new_nodes_head = NULL;
	//copy everything before the $ sign
	ft_strncpy(new_str, orig_lex_node->word, (size_t)h->i);
	new_str[h->i] = '\0';
	if (orig_lex_node->word[(h->i) + 1] == '?')	//for $? we need to handle it differently
		return (ft_handle_dollar_question(new_str, exit_code, &h->i, orig_lex_node->word));
	//new_str = ft_expand_variable(new_str, &h->i, orig_lex_node->word, h->env_copy);
	new_str = ft_expand_variable(new_str, h, &needs_expansion);
	if (!new_str)
		return (NULL);
	if (new_str[h->orig_i] == '\0' && h->orig_i == 0 && orig_lex_node->word[h->vns] == '\0')	//if it was an empty variable
		orig_lex_node->wasnt_empty_var = 0;
	
	/*else if (new_str[0] == '\0')	
		return (free(orig_lex_node->word), h->i = 0, orig_lex_node->keep_empty = 1 , new_str);*/
	if (ft_strchr(new_str, ' ') != NULL && orig_lex_node->type != WORD && orig_lex_node->type != HEREDOC)
		return (ft_print_ambig_redir(ft_get_var_name(orig_lex_node->word)), *exit_code = 1, NULL);
	if (needs_expansion == 1)	//if it has a space, so we have to make new nodes
	{
		new_nodes_head = ft_lexer_but_with_words_and_one_cmd(new_str + h->orig_i, orig_lex_node->exec_num);
		if (h->orig_i - 1 > 0)
			((t_lexer *)(new_nodes_head)->content)->word = ft_attach_beginning(((t_lexer *)(new_nodes_head)->content)->word, orig_lex_node->word, h);
		free(new_str);
		((t_lexer *)(ft_lstlast(new_nodes_head))->content)->word = ft_concat_rest(orig_lex_node->word, h, ((t_lexer *)(ft_lstlast(new_nodes_head))->content)->word, needs_expansion);
		//ft_print_lexer_list(new_nodes_head);
		return (ft_insert_new_lexed_nodes(new_nodes_head, h));
	}
	else
		return (new_str = ft_concat_rest(orig_lex_node->word, h, new_str, 0), free(orig_lex_node->word), new_str);
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

void	ft_put_node_to_end_of_cmd(t_list **orig, t_list **first_in_cmd, t_list *to_be_moved)
{
	t_lexer	*lexed_l_cont;
	t_list	*prev;
	t_list	*tmp;

	lexed_l_cont = (t_lexer *)((to_be_moved)->content);
	if (!(to_be_moved)->next || ((t_lexer *)(to_be_moved)->next->content)->exec_num != lexed_l_cont->exec_num)
		return ;
	
	prev = *first_in_cmd;
	if (to_be_moved == *first_in_cmd)
	{
		*first_in_cmd = to_be_moved->next;
		*orig = *first_in_cmd;
	}
	else
	{
		while (prev->next != to_be_moved)
			prev = prev->next;
		prev->next = (to_be_moved)->next;
		*orig = prev->next;
	}
	prev = *first_in_cmd;
	while ((prev)->next && ((t_lexer *)(prev)->next->content)->exec_num == lexed_l_cont->exec_num)
		(prev) = (prev)->next;
	tmp = prev->next;
	if (!(prev->next))
		(to_be_moved)->next = NULL;
	else
		(to_be_moved)->next = tmp;
	prev->next = to_be_moved;
	
	//ft_print_lexer_list(*orig);
	//printf("---------------------\n");
	//ft_print_lexer_list(*first_in_cmd);
}

void	ft_rearrange_lexed_list(t_list **lexed_l, int i) //put every WORD type node to the end
{
	int		max_cmds;
	int		j;
	t_list *beg;

	if (!lexed_l || !*lexed_l)
		return ;
	max_cmds = ((t_lexer *)ft_lstlast(*lexed_l)->content)->exec_num;
	while (++i <= max_cmds)
	{
		int number_of_nodes_in_cmd = ft_get_number_of_nodes_in_cmd(*lexed_l, i);
		beg = *lexed_l;
		j = 0;
		while (lexed_l && j < number_of_nodes_in_cmd)
		{
			//printf("word: %s\n", ((t_lexer *)(*lexed_l)->content)->word);
			if (((t_lexer *)(*lexed_l)->content)->type == WORD)
				ft_put_node_to_end_of_cmd(lexed_l, &beg, *lexed_l);
			else
				*lexed_l = (*lexed_l)->next;
			j++;
		}
		*lexed_l = beg;
	}
}

void init_expander_helper(t_expander_helper *h, t_list **lexed_list, t_list **env_copy)
{
	(void)lexed_list;
	h->list_head = NULL;
	h->current_node = NULL;
	//h->curr_cont = NULL;
	h->env_copy = env_copy;
	h->i = 0;
	h->orig_i = 0;
	h->vns = 0;
	h->var_value = NULL;
}

t_list	*ft_expander(t_list **lexed_list, t_list **env_copy, int exit_code)
{
	t_expander_helper	h;
	char *tmp;

	init_expander_helper(&h, lexed_list, env_copy);
	if (!(lexed_list))
		return (NULL);
	h.list_head = *lexed_list;
	h.current_node = h.list_head;
	while (h.current_node)
	{
		h.i = 0;
		while (((t_lexer *)h.current_node->content)->word[h.i] && ((t_lexer *)h.current_node->content)->type != HEREDOC)
		{
			if (((t_lexer *)h.current_node->content)->word[h.i] == '\'')
				((t_lexer *)h.current_node->content)->word = ft_remove_quote(((t_lexer *)h.current_node->content)->word, &h.i, '\'');	//no expand, no split | abc'a$b'c --> abcabc, i was 3, now 5
			else if (((t_lexer *)h.current_node->content)->word[h.i] == '\"')
				((t_lexer *)h.current_node->content)->word = ft_expand_dquote(&h, exit_code);	//expand, no split | abc"a$b"c --> abcabc, i was 3, now 3
			else if (((t_lexer *)h.current_node->content)->word[h.i] == '$')	//expand, split | insert another lexer_node if needed | abc$a
			{
				tmp = ft_expand_with_split(&h, &exit_code);
				((t_lexer *)h.current_node->content)->word = tmp;
			}
			else
				h.i++;
			if (!((t_lexer *)h.current_node->content)->word)	//malloc failed
				return (ft_free_lexer(h.list_head), h.list_head = NULL, NULL);
		}
		h.current_node = h.current_node->next;
	}
	ft_rearrange_lexed_list(&(h.list_head), -1);
	return (h.list_head);
}