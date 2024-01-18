/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 13:21:35 by rluari            #+#    #+#             */
/*   Updated: 2024/01/18 21:14:36 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>


char	*ft_get_var_value(char *var_name, t_list **env_copy)	//0, str that has the whole command with the var, str index where we encountered the $
{
	char 	*var_value;

	//2. getting the variable value
	if (!var_name)
		return (NULL);
	var_value = ft_get_env_value(*env_copy, var_name);
	free(var_name);
	if (!var_value)
		var_value = ft_strdup("");	//if variable does not exist, then var_value = ""

	return (var_value);
}

char	*ft_get_var_name(char *str)
{
	char	*res;
	int		j;

	j = 0;
	res = malloc(sizeof(char) * ft_strlen(&str[j]) + 1);
	if (!res)
		return (perror("Malloc failed"), NULL);
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

	*i += 1;	//skip the $
	vns = 0;
	//1. cutting the variable name
	while (str[*i + vns] && !ft_is_non_var_char(str[*i + vns]))	//example: ab"cd$ef"gh -->var name is "ef"
		(vns)++;
	return (vns);
}

_Bool	ft_has_word(char *str)
{
	int	i;

	i = 0;
	ft_skip_spaces(str, &i);
	if (str[i] == '\0')
		return (0);
	else
	{
		while (str[i] && (str[i] != ' '))
			i++;
		if (str[i] == '\0')
			return (0);
		ft_skip_spaces(str, &i);
		if (str[i] == '\0')
			return (0);
		else
			return (1);
	}
	return (1);
}

char	*ft_expand_variable(char *new_str, t_expander_helper *h, _Bool *needs_expansion, char *str)
{
	char	*tmp;
	int		ns_size;
	char	*var_name;

	h->orig_i = h->i;
	h->vns = ft_get_var_name_size(str, &h->i);
	var_name = ft_substr(str, (unsigned int)(h->i), (size_t)(h->vns));
	//2. getting the variable value
	h->var_value = ft_get_var_value(var_name, h->env_copy);	//0, str that has the whole command with the var, str index where we encountered the $
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
	if (needs_expansion && ft_has_word(h->var_value))
		*needs_expansion = 1;
	return (new_str);
}

char *ft_insert_new_lexed_nodes(t_list *new_nodes_head, t_expander_helper *h)
{
	t_list *new_current;
	t_list *tmp;

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
	//h->curr_cont = (t_lexer *)(h->current_node)->content;
	//h->i = ft_strlen(((t_lexer *)new_current->content)->word);
	return (((t_lexer *)h->current_node->content)->word);
}

void	ft_print_ambig_redir(char *var_name)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(var_name, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
}

char	*ft_handle_dollar_question(char *new_str, int *exit_code, int *i, char *str)
{
	char	*tmp;
	char	*exit_code_str;

	exit_code_str = ft_itoa(*exit_code);
	int asd = ft_strlen(new_str) + ft_strlen(exit_code_str) + ft_strlen(&str[*i + 2]) + 1;
	tmp = malloc(sizeof(char) * (ft_strlen(new_str) + ft_strlen(exit_code_str) + ft_strlen(&str[*i + 2]) + 1));
	(void)asd;
	if (!tmp || !exit_code_str)
		return (perror("Malloc failed"), NULL);
	ft_strcpy(tmp, new_str);
	ft_strlcat(tmp, exit_code_str, ft_strlen(new_str) + ft_strlen(exit_code_str) + 1);
	ft_strlcat(tmp, str + *i + 2, ft_strlen(tmp) + ft_strlen(&str[*i + 2]) + 1);
	*i += ft_strlen(exit_code_str);
	free(str);
	free(new_str);
	free(exit_code_str);
	new_str = tmp;
	return (new_str);
}
char	*ft_handle_dollar_question_q(char *new_str, int *exit_code, int *i, char *str)
{
	char	*tmp;
	char	*exit_code_str;

	exit_code_str = ft_itoa(*exit_code);
	tmp = malloc(sizeof(char) * ( ft_strlen(new_str) + ft_strlen(exit_code_str) + ft_strlen(&str[*i + 2]) )); // -1 for the last ", -2 for the $?, +1 for the \0
	if (!tmp || !exit_code_str)
		return (perror("Malloc failed"), NULL);
	ft_strcpy(tmp, new_str);
	ft_strlcat(tmp, exit_code_str, ft_strlen(new_str) + ft_strlen(exit_code_str) + 1);
	//ft_strlcat(tmp, str + *i + 2, ft_strlen(tmp) + ft_strlen(&str[*i + 2]) + 1);
	*i += 2;
	//*exit_code = 0;
	free(new_str);
	free(exit_code_str);
	new_str = tmp;
	return (new_str);
}

//delete later
/*_Bool	ft_add_back_to_new_ll(t_list **new_lexed_list, t_list *lexed_list, _Bool num)
{
	t_list *new_tmp;
	//t_lexer	*new_content;

	//new_content = (t_lexer *)malloc(sizeof(t_lexer));
	new_content->word = ft_strdup(((t_lexer *)lexed_list->content)->word);
	if (new_content->word == NULL)
		return (1);	//free ll?
	new_content->type = ((t_lexer *)lexed_list->content)->type;
	new_content->exec_num = ((t_lexer *)lexed_list->content)->exec_num;
	new_tmp = ft_lstnew(lexed_list->content);
	if ((((t_lexer *)lexed_list->content)->type != WORD) == num)
		ft_lstadd_back(new_lexed_list, new_tmp);
	if (new_tmp == NULL)
	{
		ft_free_lexer(*new_lexed_list);
		ft_free_lexer(lexed_list);
		return (1);
	}
	return (0);
}
*/
