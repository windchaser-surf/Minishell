/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 10:37:28 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 18:16:39 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

char	*ft_remove_quote(char *str, int *i, char c)
{
	char	*tmp;
	int		j;

	tmp = malloc(sizeof(char) * ft_strlen(str) - 2 + 1);
	if (!tmp)
		return (NULL);
	ft_strncpy(tmp, str, (size_t)*i);
	(*i)++;
	while (str[*i] && str[*i] != c)
	{
		tmp[*i - 1] = str[*i];
		*i += 1;
	}
	(*i)++;
	j = 0;
	while (str[*i + j])
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

/*har	*ft_expand_tilde(char *new_str, t_expander_helper *h, char *str)
{
	char	*tmp;
	int		ns_size;

	h->orig_i = h->i;
	h->vns = 4;
	//2. getting the variable value
	h->var_value = ft_get_var_value("HOME", h->env_copy, 0);	//0, str that has the whole command with the var, str index where we encountered the $
	if (!h->var_value)
		return (perror("Malloc failed"), NULL);
	//3. concatenating it to the end of the string and removing the $var_name
	ns_size = ft_strlen(new_str);
	tmp = malloc(ns_size + ft_strlen(&str[h->i]) - (++h->vns) + ft_strlen(h->var_value) + 2);	//update len with the variable value
	if (!tmp)
		return (perror("Malloc failed"), NULL);
	ft_strcpy(tmp, new_str);	//copy everything before the $
	free(new_str);
	new_str = tmp;
	ft_strlcat(new_str, h->var_value, ns_size + ft_strlen(h->var_value) + 1);	//append the variable value to the end of the string
	h->i = h->orig_i + ft_strlen(h->var_value);	
	//we set i to the position of the last char of the variable name (not value !)

	return (new_str);
}*/

char	*ft_expand_dquote(t_expander_helper *h, int exit_code)
{
	char	*new_str;
	int		j;
	char	*str;

	str = ((t_lexer *)h->current_node->content)->word;
	new_str = ft_make_new_str_for_expanstion(&h->i, str, &j);
	if (!new_str)
		return (NULL);
	while (str[h->i] && str[h->i] != '\"')
	{
		if (str[h->i] == '$' && ft_is_var_char(str[h->i]))
		{
			if (str[(h->i) + 1] == '?')
				new_str = ft_handle_dollar_question_q(new_str, &exit_code, &h->i, str);
			else
			{
				new_str = ft_expand_variable(new_str, h, NULL, str);
				h->i = h->orig_i + h->vns;
			}
			j = ft_strlen(new_str);
		}
		else
			new_str[j++] = str[(h->i)++];
		new_str[j] = '\0';
	}
	ft_copy_whats_after_the_quote(&h->i, &j, str, new_str);
	return (new_str);
}

t_list	*ft_lexer_but_with_words_and_one_cmd(char *command, int cmd_num)
{
	t_lexer_helper	helper;

	ft_init_lexer_helper(&helper, command, cmd_num);
	while (command[helper.i])
	{
		if (command[helper.i] == ' ')
		{
			if (ft_handle_lexer_word(&helper, command, NULL))
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
	rest = ft_substr(str, h->orig_i + h->vns,
		ft_strlen(str) - (h->orig_i + h->vns));
	if (!rest)
		return (perror("Malloc failed"), NULL);
	new_str = ft_strjoin_free(new_str, rest);
	free(rest);
	return (new_str);
}

char	*ft_attach_beginning(char *head_node_str, char *str, t_expander_helper *h)
{
	char	*tmp;

	tmp = malloc(sizeof(char) * ft_strlen(head_node_str) + h->orig_i + 1);
	if (!tmp)
		return (perror("Malloc failed"), NULL);
	ft_strlcpy(tmp, str, (size_t)h->orig_i + 1);
	ft_strlcat(tmp, head_node_str, ft_strlen(head_node_str) + h->orig_i + 1);
	free(head_node_str);
	return (tmp);
}

_Bool	ft_is_var_char(char c)
{
	if (c == '\0')
		return (0);
	if (ft_isalnum(c) || c == '_' || c == '?' /*|| c == '$'*/)
			return (0);
	return (1);
}

char	*ft_expand_with_split(t_expander_helper *h, int *exit_code)
{
	char	*new_str;
	t_lexer *orig_lex_node;
	t_list	*new_nodes_head;

	h->needs_expansion = 0;
	orig_lex_node  = (t_lexer *)(h->current_node)->content;
	if (orig_lex_node->word[h->i + 1] == '\0' || orig_lex_node->word[h->i + 1] == '$' )
		return ((h->i)++, orig_lex_node->word);
	new_str = malloc(sizeof(char) * ft_strlen(orig_lex_node->word) + 1);
	if (!new_str)
		return (perror("Malloc failed"), NULL);
	new_nodes_head = NULL;
	//copy everything before the $ sign
	ft_strncpy(new_str, orig_lex_node->word, (size_t)h->i);
	new_str[h->i] = '\0';
	if (orig_lex_node->word[(h->i) + 1] == '?')
		return (ft_handle_dollar_question(new_str, exit_code, &h->i, orig_lex_node->word));
	new_str = ft_expand_variable(new_str, h, &h->needs_expansion, orig_lex_node->word);
	if (!new_str)
		return (NULL);
	if (new_str[h->orig_i] == '\0' && h->orig_i == 0 && orig_lex_node->word[h->vns] == '\0')
		orig_lex_node->wasnt_empty_var = 0;
	if (ft_strchr(new_str, ' ') != NULL && orig_lex_node->type != WORD && orig_lex_node->type != HEREDOC)
		return (ft_print_ambig_redir(ft_get_var_name(orig_lex_node->word)), *exit_code = 1, NULL);
	if (h->needs_expansion == 1)
	{
		new_nodes_head = ft_lexer_but_with_words_and_one_cmd(new_str + h->orig_i, orig_lex_node->exec_num);
		if (h->orig_i > 0)
			((t_lexer *)(new_nodes_head)->content)->word = ft_attach_beginning(((t_lexer *)(new_nodes_head)->content)->word, orig_lex_node->word, h);
		free(new_str);
		((t_lexer *)(ft_lstlast(new_nodes_head))->content)->word = ft_concat_rest(orig_lex_node->word, h, ((t_lexer *)(ft_lstlast(new_nodes_head))->content)->word, h->needs_expansion);
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

void	ft_put_node_to_end_of_cmd(t_list **orig, t_list **first_in_cmd, t_list *t_b_m)
{
	t_lexer	*lexed_l_cont;
	t_list	*prev;

	lexed_l_cont = (t_lexer *)(t_b_m)->content;
	if (!(t_b_m)->next || ((t_lexer *)(t_b_m)->next->content)->exec_num != lexed_l_cont->exec_num)
		return ;
	prev = *first_in_cmd;
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
	prev = *first_in_cmd;
	while ((prev)->next && ((t_lexer *)(prev)->next->content)->exec_num == lexed_l_cont->exec_num)
		(prev) = (prev)->next;
	if (!(prev->next))
		(t_b_m)->next = NULL;
	else
		(t_b_m)->next = prev->next;
	prev->next = t_b_m;
}

void	ft_rearrange_lexed_list(t_list **lexed_l, int i, t_expander_helper *h)
{
	int		max_cmds;
	int		j;
	t_list *beg;

	(void)h;
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
	h->list_head = *lexed_list;
	h->current_node = h->list_head;
	h->env_copy = env_copy;
	h->i = 0;
	h->orig_i = 0;
	h->vns = 0;
	h->var_value = NULL;
	h->needs_expansion = 0;
}

/*static void	ft_expander_while(char *str, t_expander_helper *h, int *exit_code)
{
	if (str[h->i] == '\'')
		str = ft_remove_quote(str, &h->i, '\'');
	else if (str[h->i] == '\"')
		str = ft_expand_dquote(h, *exit_code);
	else if (str[h->i] == '$')
		str = ft_expand_with_split(h, exit_code);
	else
		h->i++;
}*/

t_list	*ft_expander(t_list **lexed_list, t_list **env_copy, int exit_code)
{
	t_expander_helper	h;
	char				*str;

	init_expander_helper(&h, lexed_list, env_copy);
	if (!(lexed_list))
		return (NULL);
	while (h.current_node)
	{
		h.i = 0;
		str = ((t_lexer *)h.current_node->content)->word;
		while (str[h.i] &&
			((t_lexer *)h.current_node->content)->type != HEREDOC)
		{
			//ft_expander_while(str, &h, &exit_code);
			if (str[h.i] == '\'')
				str = ft_remove_quote(str, &h.i, '\'');
			else if (str[h.i] == '\"')
				str = ft_expand_dquote(&h, exit_code);
			else if (str[h.i] == '$')
				str = ft_expand_with_split(&h, &exit_code);
			else
				h.i++;
			((t_lexer *)h.current_node->content)->word = str;
			if (!((t_lexer *)h.current_node->content)->word)
				return (ft_free_lexer(h.list_head), h.list_head = NULL, NULL);
		}
		h.current_node = h.current_node->next;
	}
	ft_rearrange_lexed_list(&(h.list_head), -1, &h);
	return (h.list_head);
}