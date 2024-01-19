/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 13:21:35 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 18:20:22 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>


char	*ft_get_var_value(char *var_name, t_list **env_copy, _Bool need_free)
{
	char 	*var_value;

	if (!var_name)
		return (NULL);
	var_value = ft_get_env_value(*env_copy, var_name);
	if (need_free)
		free(var_name);
	if (!var_value)
		var_value = ft_strdup("");

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

	*i += 1;
	vns = 0;
	while (str[*i + vns] && !ft_is_var_char(str[*i + vns]))
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
	h->var_value = ft_get_var_value(var_name, h->env_copy, 1);
	if (!h->var_value)
		return (free(h->var_value), perror("Malloc failed"), NULL);
	ns_size = ft_strlen(new_str);
	tmp = malloc(ns_size + ft_strlen(&str[h->i]) - (++h->vns)
		+ ft_strlen(h->var_value) + 2);
	if (!tmp)
		return (free(h->var_value), perror("Malloc failed"), NULL);
	ft_strcpy(tmp, new_str);
	free(new_str);
	new_str = tmp;
	ft_strlcat(new_str, h->var_value, ns_size + ft_strlen(h->var_value) + 1);
	h->i = h->orig_i + ft_strlen(h->var_value);	
	if (needs_expansion && ft_has_word(h->var_value))
		*needs_expansion = 1;
	free(h->var_value);
	return (new_str);
}

char *ft_insert_new_lexed_nodes(t_list *new_nodes_head, t_expander_helper *h)
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

void	ft_print_ambig_redir(char *var_name)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(var_name, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
}

char	*ft_handle_dollar_question(char *n_s, int *exit_code, int *i, char *str)
{
	char	*tmp;
	char	*ecs;

	ecs = ft_itoa(*exit_code);
	int asd = ft_strlen(n_s) + ft_strlen(ecs)	+ ft_strlen(&str[*i + 2]) + 1;
	tmp = malloc(sizeof(char) * (ft_strlen(n_s)
		+ ft_strlen(ecs) + ft_strlen(&str[*i + 2]) + 1));
	(void)asd;
	if (!tmp || !ecs)
		return (ft_putstr_fd(EMSG_MAL, 2), NULL);
	ft_strcpy(tmp, n_s);
	ft_strlcat(tmp, ecs, ft_strlen(n_s) + ft_strlen(ecs) + 1);
	ft_strlcat(tmp, str + *i + 2, ft_strlen(tmp)
		+ ft_strlen(&str[*i + 2]) + 1);
	*i += ft_strlen(ecs);
	free(str);
	free(n_s);
	free(ecs);
	n_s = tmp;
	return (n_s);
}
char	*ft_handle_dollar_question_q(char *new_str, int *exit_code, int *i, char *str)
{
	char	*tmp;
	char	*exit_code_str;

	exit_code_str = ft_itoa(*exit_code);
	tmp = malloc(sizeof(char) * (ft_strlen(new_str) +
		ft_strlen(exit_code_str) + ft_strlen(&str[*i + 2])));
	if (!tmp || !exit_code_str)
		return (perror("Malloc failed"), NULL);
	ft_strcpy(tmp, new_str);
	ft_strlcat(tmp, exit_code_str, ft_strlen(new_str) + ft_strlen(exit_code_str) + 1);
	*i += 2;
	free(new_str);
	free(exit_code_str);
	new_str = tmp;
	return (new_str);
}
