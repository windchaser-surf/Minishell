/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 10:37:28 by rluari            #+#    #+#             */
/*   Updated: 2024/01/07 12:15:11 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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


char	*ft_expand_dquote(char *str, int *i, t_list **env_copy, int exit_code)	//from ab"cde"fg"hi" to abcdefghi, and setting i to the position of f
{
	char	*new_str;
	int		j;
	
	new_str = malloc(sizeof(char) * ft_strlen(str) - 2 + 1);
	if(!new_str)
		return (perror("Malloc failed"), NULL);
	ft_strncpy(new_str, str, (size_t)*i);	//copy everything before the quote
	new_str[*i] = '\0';
	(*i)++;	//skip the quote
	j = *i - 1;
	while (str[*i] && str[*i] != '\"')	//copy everything between the quotes
	{
		if (str[*i] == '$' && (ft_isalpha(str[(*i) + 1])
			|| str[(*i) + 1] == '?' || str[(*i) + 1] == '_'))	//expand variable if it is not the last char or another $
		{
			if (str[(*i) + 1] == '?')
				new_str = ft_handle_dollar_question_q(new_str, &exit_code, i, str);
			else
				new_str = ft_expand_variable(new_str, i, str, env_copy);	//it expands and attaches to the end of the string
			j = ft_strlen(new_str);
		}
		else
		{
			new_str[j++] = str[(*i)++];
			new_str[j] = '\0';
		}
	}
	(*i)++;	//skip the closing quote
	int tmp_i = *i;
	*i = j;
	while (str[tmp_i])	//copy everything after the quote
		new_str[j++] = str[tmp_i++];
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

t_list	*ft_lexer_but_with_words_and_one_cmd(char *command)
{
	t_lexer_helper	helper;

	ft_init_lexer_helper(&helper, command);
	ft_skip_spaces(command, &helper.i);
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

char	*ft_expand_with_split(t_expander_helper *h, int *exit_code)
{
	char	*new_str;
	t_lexer *orig_lex_node;
	t_list	*new_nodes_head;

	orig_lex_node  = (t_lexer *)(h->current_node)->content;	//we are in this one, only in this
	if (orig_lex_node->word[h->i + 1] == '\0' || orig_lex_node->word[h->i + 1] == '$')	//if it is the last char or another $, then it is not a variable, so we return
		return ((h->i)++, orig_lex_node->word);
	new_str = malloc(sizeof(char) * ft_strlen(orig_lex_node->word) + 1);
	if (!new_str)
		return (perror("Malloc failed"), NULL);
	new_nodes_head = NULL;
	ft_strncpy(new_str, orig_lex_node->word, (size_t)h->i);	//copy everything before the $ sign
	new_str[h->i] = '\0';
	if (orig_lex_node->word[(h->i) + 1] == '?')
		return (ft_handle_dollar_question(new_str, exit_code, &h->i, orig_lex_node->word));
	new_str = ft_expand_variable(new_str, &h->i, orig_lex_node->word, h->env_copy);
	if (!new_str[0])
		return (free(orig_lex_node->word), h->i = 0, new_str);
	if (ft_strchr(new_str, ' ') != NULL && orig_lex_node->type != WORD && orig_lex_node->type != HEREDOC)
		return (ft_print_ambig_redir(ft_get_var_name(orig_lex_node->word)), *exit_code = 1, NULL);
	//create the new lexed list's nodes based on the actual new_str (without the rest after the $)
	//new_nodes_head = ft_lexer(new_str);	//including the original, so we need to free that
	new_nodes_head = ft_lexer_but_with_words_and_one_cmd(new_str);
	
	free(new_str);
	//get the the last node of the new lexed list

	return (ft_insert_new_lexed_nodes(new_nodes_head, &h->current_node, &h->list_head, &h->i));
}

void	ft_put_node_to_end_of_cmd(t_list *lexed_l, int number_of_nodes_in_cmd)
{
	(void)number_of_nodes_in_cmd;
	(void)lexed_l;
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

void	ft_rearrange_lexed_list(t_list **lexed_l, int i) //put every WORD type node to the end
{
	int		max_cmds;
	int		j;
	t_list *beg;

	max_cmds = ((t_lexer *)ft_lstlast(*lexed_l)->content)->exec_num;
	while (++i <= max_cmds)
	{
		int number_of_nodes_in_cmd = ft_get_number_of_nodes_in_cmd(*lexed_l, i);
		beg = *lexed_l;
		j = 0;
		while (lexed_l && j < number_of_nodes_in_cmd)
		{
			//if (ft_add_back_to_new_ll(&new_lexed_list, lexed_l, 1))
			ft_put_node_to_end_of_cmd(*lexed_l, number_of_nodes_in_cmd);
			*lexed_l = (*lexed_l)->next;
			j++;
		}
		*lexed_l = beg;
	}
	
}

void init_expander_helper(t_expander_helper *h, t_list **lexed_list, t_list **env_copy, int i)
{
	(void)lexed_list;
	h->list_head = NULL;
	h->current_node = NULL;
	h->env_copy = env_copy;
	h->i = i;
}

t_list	*ft_expander(t_list **lexed_list, t_list **env_copy, int exit_code, int i)
{
	t_expander_helper	h;
	t_lexer	*lexer_node;

	init_expander_helper(&h, lexed_list, env_copy, i);
	printf("---------------------BEFORE rearrange:\n");
	ft_print_lexer_list(*lexed_list);
	ft_rearrange_lexed_list(lexed_list, -1);
	h.list_head = *lexed_list;

	h.current_node = h.list_head;
	while (h.current_node)
	{
		lexer_node = (t_lexer *)h.current_node->content;
		h.i = 0;
		while (lexer_node->word[h.i] && lexer_node->type != HEREDOC)
		{
			if (lexer_node->word[h.i] == '\'')
				lexer_node->word = ft_remove_quote(lexer_node->word, &h.i, '\'');	//no expand, no split | abc'a$b'c --> abcabc, i was 3, now 5
			else if (lexer_node->word[h.i] == '\"')
				lexer_node->word = ft_expand_dquote(lexer_node->word, &h.i, h.env_copy, exit_code);	//expand, no split | abc"a$b"c --> abcabc, i was 3, now 3
			else if (lexer_node->word[h.i] == '$')	//expand, split | insert another lexer_node if needed | abc$a
				lexer_node->word = ft_expand_with_split(&h, &exit_code);
			else
				h.i++;
			if (!lexer_node->word)	//malloc failed
				return (ft_free_lexer(h.list_head), h.list_head = NULL, NULL);
		}
		h.current_node = h.current_node->next;
	}
	/*printf("---------------------AFTER rearrange:\n");
	ft_print_lexer_list(h.list_head);*/
	//free(*lexed_list);
	return (h.list_head);
}