/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 10:37:28 by rluari            #+#    #+#             */
/*   Updated: 2023/12/17 18:44:47 by rluari           ###   ########.fr       */
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

char	*ft_get_var_value(int *vns, char *str, int i, t_list **env_copy)	//0, str that has the whole command with the var, str index where we encountered the $
{
	char 	*var_name;
	char 	*var_value;
	
	//1. cutting the variable name
	while (str[i + *vns] && (ft_isalnum(str[i + *vns]) || str[i + *vns] == '_'))	//example: ab"cd$ef"gh -->var name is "ef"
		(*vns)++;
	var_name = ft_substr(str, (unsigned int)i, (size_t)(*vns));
	var_value = ft_get_env_value(env_copy, var_name);
	free(var_name);
	if (!var_value)
		var_value = ft_strdup("");	//if variable does not exist, then var_value = ""

	return (var_value);
}

char	*ft_expand_variable(char *new_str, int *i, char *str, t_list **env_copy)
{
	char	*tmp;
	int		vns;
	char 	*var_value;
	int		new_str_original_size;

	*i += 1;	//skip the $
	
	//1. cutting the variable name
	var_value = ft_get_var_value(&vns, str, *i, env_copy);	//0, str that has the whole command with the var, str index where we encountered the $

	//2. concatenating its value to the end of the string and removing the $var_name
	new_str_original_size = ft_strlen(new_str);
	int deletelater = new_str_original_size + ft_strlen(&str[*i]) + 2;
	tmp = malloc(deletelater);	//strlen - two quotes + 1 nullterm + var_val_size - 1 for $ - variable_size
	if (!tmp)
		return NULL;
	ft_strcpy(tmp, new_str);	//copy everything before the $
	free(new_str);
	new_str = tmp;
	
	ft_strlcat(new_str, var_value, new_str_original_size + ft_strlen(var_value) + 1);	//append the variable value to the end of the string
	free(var_value);
	
	*i = *i + vns;	//we set i to the position of the last char of the variable name (not value !)
	return (new_str);
}

char	*ft_expand_dquote(char *str, int *i, t_list **env_copy)	//from ab"cde"fg"hi" to abcdefghi, and setting i to the position of f
{
	char	*new_str;
	int		new_str_size;
	int		j;
	
	new_str = malloc(sizeof(char) * ft_strlen(str) - 2 + 1);
	if(!new_str)
		return NULL;
	ft_strncpy(new_str, str, (size_t)*i);	//copy everything before the quote
	new_str[*i] = '\0';
	
	(*i)++;	//skip the quote
	new_str_size = ft_strlen(new_str);
	j = *i - 1;
	while (str[*i] && str[*i] != '\"')	//copy everything between the quotes
	{
		if (str[*i] == '$')	//expand variable
		{
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
	while (str[tmp_i])	//copy everything after the quote
	{
		new_str[j] = str[tmp_i];
		tmp_i++;
		j++;
	}
	new_str[j] = '\0';
	*i = *i - 2;
	free(str);
	return (new_str);
}

void insertList(t_list *insertAfter, t_list *newList) {
    if (!insertAfter || !newList) {
        printf("Invalid input.\n");
        return;
    }

    // Insert the new list
    newList->next = insertAfter->next;
    insertAfter->next = newList;
}

void	ft_expand_with_split(t_list **lexer_node, int *i, t_list **env_copy, t_lexer *actual_node)
{
	//1.get_var_name
	//2.get_var_value
	//if var_value == NULL, then var_value = ""
	//3.if var value has a space AND not word or heredoc, then ERROR
	//3.if var value has a space, then split, and insert new t_lexer nodes
	
	char	**val_content;
	char	*new_str;
	int		k;
	t_lexer	*tmp;
	t_list	*new_nodes_head;
	
	new_str = malloc(sizeof(char) * ft_strlen(actual_node->word) + 1);
	if (!new_str)
		return ;
	new_nodes_head = NULL;
	ft_strncpy(new_str, actual_node->word, (size_t)*i);	//copy everything before the quote
	new_str[*i] = '\0';
	new_str = ft_expand_variable(new_str, i, actual_node->word, env_copy);
	if (ft_strchr(new_str, ' ') != NULL && (actual_node->type != WORD || actual_node->type != HEREDOC))
	{
		printf("ERROR: bash: $hd: ambiguous redirect\n");	//TODO
		return ;
	}

	new_nodes_head = ft_lexer(new_str);

	//insert new nodes to the original lexer_node list
	insertList(*lexer_node - 1, new_nodes_head);
	
	//free actual node and make the pervious "next" point to the new_nodes_head
	///TODO
	
}

void	ft_expander(t_list **lexed_list, t_list **env_copy)
{
	t_list	*tmp;
	t_lexer	*lexer_node;
	int		i;

	tmp = *lexed_list;
	while (tmp)
	{
		lexer_node = (t_lexer *)tmp->content;
		i = 0;
		if (lexer_node->type != HEREDOC)	//make shorter with only one while without if
		{
			while (lexer_node->word[i])
			{
				if (lexer_node->word[i] == '\'')
				{
					lexer_node->word = ft_remove_quote(lexer_node->word, &i, '\'');	//no expand, no split | abc'a$b'c --> abcabc, i was 3, now 5
				}
				else if (lexer_node->word[i] == '\"')
				{
					lexer_node->word = ft_expand_dquote(lexer_node->word, &i, env_copy);	//expand, no split | abc"a$b"c --> abcabc, i was 3, now 3
				}
				else if (lexer_node->word[i] == '$')	//expand, split | insert another lexer_node if needed | abc$a
				{
					//if has space (splitable) AND not word or heredoc, then error: bash: $hd: ambiguous redirect
					ft_expand_with_split(lexed_list, &i, env_copy, lexer_node);	
				}
				else
					i++;
			}
		}
		printf("word: %s\n", lexer_node->word);
		tmp = tmp->next;
	}
}