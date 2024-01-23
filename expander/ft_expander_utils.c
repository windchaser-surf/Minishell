/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 20:29:42 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/23 11:00:09 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_remove_quote(char *str, int *i, char c)
{
	char	*tmp;
	int		j;

	tmp = malloc(sizeof(char) * ft_strlen(str) - 2 + 1);
	if (!tmp)
		return (NULL);
	ft_strncpy(tmp, str, (size_t)(*i));
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
	if (!(new_str))
		return (ft_putstr_fd(EMSG_MAL, 2), NULL);
	ft_strncpy(new_str, str, (size_t)(*i));
	new_str[*i] = '\0';
	(*i)++;
	*j = *i - 1;
	return (new_str);
}

void	ft_copy_whats_after_the_quote(int *i, int *j, char *str, char *new_str)
{
	int	tmp_i;

	(*i)++;
	tmp_i = *i;
	*i = *j;
	while (str[tmp_i])
		new_str[(*j)++] = str[tmp_i++];
	new_str[*j] = '\0';
	free(str);
}

t_ex_h	*ft_checker_expand_dqoute(t_ex_h *h, \
	char **new_str, char *str)
{
	*new_str = ft_expand_variable(*new_str, h, NULL, str);
	h->i = h->orig_i + h->vns;
	return (h);
}

char	*ft_expand_dquote(t_ex_h *h, int exit_code)
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
		if (str[h->i] == '$' && ft_is_var_char(str[h->i + 1]))
		{
			if (str[(h->i) + 1] == '?')
				new_str = ft_handle_dollar_question_q(new_str, \
					&exit_code, &h->i, str);
			else
				h = ft_checker_expand_dqoute(h, &new_str, str);
			j = ft_strlen(new_str);
		}
		else
			new_str[j++] = str[(h->i)++];
		new_str[j] = '\0';
	}
	return (ft_copy_whats_after_the_quote(&h->i, &j, str, new_str), new_str);
}
