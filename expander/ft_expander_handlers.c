/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 13:21:35 by rluari            #+#    #+#             */
/*   Updated: 2024/01/07 10:55:21 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>


char	*ft_get_var_value(char *var_name, t_list **env_copy)	//0, str that has the whole command with the var, str index where we encountered the $
{
	char 	*var_value;

	//2. getting the variable value
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

char	*ft_expand_variable(char *new_str, int *i, char *str, t_list **env_copy)
{
	char	*tmp;
	int		vns;
	char 	*var_value;
	int		new_str_original_size;
	char	*var_name;
	
	*i += 1;	//skip the $
	vns = 0;
	//1. cutting the variable name
	while (str[*i + vns] && (ft_isalnum(str[*i + vns]) || str[*i + vns] == '_'))	//example: ab"cd$ef"gh -->var name is "ef"
		(vns)++;
	var_name = ft_substr(str, (unsigned int)(*i), (size_t)(vns));
	
	//2. getting the variable value
	var_value = ft_get_var_value(var_name, env_copy);	//0, str that has the whole command with the var, str index where we encountered the $
	
	//3. concatenating it to the end of the string and removing the $var_name
	new_str_original_size = ft_strlen(new_str);
	tmp = malloc(new_str_original_size + ft_strlen(&str[*i]) - (vns + 1) + ft_strlen(var_value) + 2);	//update len with the variable value
	if (!tmp)
		return (perror("Malloc failed"), NULL);
	ft_strcpy(tmp, new_str);	//copy everything before the $
	free(new_str);
	new_str = tmp;
	
	ft_strlcat(new_str, var_value, new_str_original_size + ft_strlen(var_value) + 1);	//append the variable value to the end of the string
	free(var_value);
	
	*i = *i + vns;	//we set i to the position of the last char of the variable name (not value !)
	return (new_str);
}

void	ft_do_the_swapping(t_list *new_nodes_head, t_list *current_node, t_list **lexed_list_head, t_list *new_current)
{
	t_list *tmp;
	
	tmp = *lexed_list_head;
	if (tmp == current_node)
		*lexed_list_head = new_nodes_head;
	else
	{
		while (tmp->next != current_node && tmp->next)
			tmp = tmp->next;
		tmp->next = new_nodes_head;
	}	
	if (current_node->next != NULL)
		new_current->next = current_node->next;
	//ft_free_lexer_node(current_node);
	
	printf("AFTER SWAPPING, the new nodes are:\n");
	ft_print_lexer_list(*lexed_list_head);
	/*tmp2 = new_nodes_head;
	while (tmp2->next != new_current && tmp2->next)	//stop one before the last node of the newly created lexed list
		tmp2 = tmp2->next;
	if (tmp2->next == NULL && *lexed_list_head == new_nodes_head)
		*lexed_list_head = *current_node;
	else if (tmp2->next == NULL)
		tmp->next = *current_node;
	else
		tmp2->next = *current_node;	//make the one before the last node of the newly created lexed list point to the original's next node
	printf("AFTER SWAPPING, the new nodes are:\n");
	ft_print_lexer_list(*lexed_list_head);*/
}

char *ft_insert_new_lexed_nodes(t_list *new_nodes_head, t_list **current_node, t_list **lexed_list_head, int *i)
{
	t_list *new_current;
	//t_lexer *original_lexer_node = (t_lexer *)current_node->content;
	//int j;
	
	//make new_nodes_head's last node to point at the original's next node
	new_current = ft_lstlast(new_nodes_head);
	
	//int original_word_len = ft_strlen(((t_lexer *)new_current->content)->word);	//the length of the last node's word before realloc
	/*char *tmp_word = malloc(sizeof(char) * (ft_strlen(new_current_lexer->word) + ft_strlen(&original_lexer_node->word[*i]) + 1));
	if (!tmp_word)
		return ;
	ft_strcpy(tmp_word, new_current_lexer->word);	//copy the last new lexed node's word into the temp
	free(new_current_lexer->word);
	new_current_lexer->word = tmp_word;
	
	j = original_word_len;
	int tmp_i = *i;
	while (original_lexer_node->word[tmp_i])
		new_current_lexer->word[j++] = original_lexer_node->word[tmp_i++];

	new_current_lexer->word[j] = '\0';*/

	ft_do_the_swapping(new_nodes_head, *current_node, lexed_list_head, new_current);
	//make the current_node point to the new_nodes_head
	*current_node = new_current;
	
	//make the node before the current_node point to the new_nodes_head
	//original_lexer_node->word = new_current_lexer->word;
	
	*i = ft_strlen(((t_lexer *)new_current->content)->word);
	return (((t_lexer *)new_current->content)->word);
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
	//*exit_code = 0;
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

_Bool	ft_add_back_to_new_ll(t_list **new_lexed_list, t_list *lexed_list, _Bool num)
{
	t_list *new_tmp;
	//t_lexer	*new_content;

	//new_content = (t_lexer *)malloc(sizeof(t_lexer));
	/*new_content->word = ft_strdup(((t_lexer *)lexed_list->content)->word);
	if (new_content->word == NULL)
		return (1);	//free ll?
	new_content->type = ((t_lexer *)lexed_list->content)->type;
	new_content->exec_num = ((t_lexer *)lexed_list->content)->exec_num;*/
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

