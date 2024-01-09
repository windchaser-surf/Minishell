/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felix <felix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 10:37:28 by rluari            #+#    #+#             */
/*   Updated: 2024/01/09 11:59:22 by felix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
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

char	*ft_expand_dquote(char *str, int *i, t_list **env_copy, int exit_code)	//from ab"cde"fg"hi" to abcdefghi, and setting i to the position of f
{
	char	*new_str;
	int		j;

	new_str = ft_make_new_str_for_expanstion(i, str, &j);
	if (!new_str)
		return (NULL);
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
	ft_copy_whats_after_the_quote(i, &j, str, new_str);
	return (new_str);
}

t_list	*ft_lexer_but_with_words_and_one_cmd(char *command, int cmd_num)
{
	t_lexer_helper	helper;

	ft_init_lexer_helper(&helper, command, cmd_num);
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
	new_nodes_head = ft_lexer_but_with_words_and_one_cmd(new_str, orig_lex_node->exec_num);
	free(new_str);
	//get the the last node of the new lexed list
	return (ft_insert_new_lexed_nodes(new_nodes_head, h)); //&h->current_node, &h->list_head, &h->i)
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
}

t_list	*ft_expander(t_list **lexed_list, t_list **env_copy, int exit_code)
{
	t_expander_helper	h;
	char *tmp;

	init_expander_helper(&h, lexed_list, env_copy);
	/*printf("---------------------BEFORE rearrange:\n");
	ft_print_lexer_list(*lexed_list);*/
	if (!(lexed_list))
		return (NULL);
	h.list_head = *lexed_list;
	h.current_node = h.list_head;
	while (h.current_node)
	{
		//h.curr_cont = (t_lexer *)h.current_node->content;
		h.i = 0;
		while (((t_lexer *)h.current_node->content)->word[h.i] && ((t_lexer *)h.current_node->content)->type != HEREDOC)
		{
			if (((t_lexer *)h.current_node->content)->word[h.i] == '\'')
				((t_lexer *)h.current_node->content)->word = ft_remove_quote(((t_lexer *)h.current_node->content)->word, &h.i, '\'');	//no expand, no split | abc'a$b'c --> abcabc, i was 3, now 5
			else if (((t_lexer *)h.current_node->content)->word[h.i] == '\"')
				((t_lexer *)h.current_node->content)->word = ft_expand_dquote(((t_lexer *)h.current_node->content)->word, &h.i, h.env_copy, exit_code);	//expand, no split | abc"a$b"c --> abcabc, i was 3, now 3
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