/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 13:21:35 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 22:02:35 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_var_value(char *var_name, t_list **env_copy, _Bool need_free)
{
	char	*var_value;

	if (!var_name)
		return (NULL);
	var_value = ft_get_env_value(*env_copy, var_name);
	if (need_free)
		free(var_name);
	if (!var_value)
		var_value = ft_strdup("");
	return (var_value);
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

char	*ft_expand_variable(char *new_str, t_ex_h *h,
	_Bool *needs_exp, char *str)
{
	char	*tmp;
	int		ns_size;
	char	*var_name;

	h->orig_i = h->i;
	h->vns = ft_get_var_name_size(str, &h->i);
	var_name = ft_substr(str, (unsigned int)(h->i), (size_t)(h->vns));
	h->var_value = ft_get_var_value(var_name, h->env_copy, 1);
	if (!h->var_value)
		return (free(h->var_value), ft_putstr_fd(EMSG_MAL, 2), NULL);
	ns_size = ft_strlen(new_str);
	tmp = malloc(ns_size + ft_strlen(&str[h->i]) - (++h->vns)
			+ ft_strlen(h->var_value) + 2);
	if (!tmp)
		return (free(h->var_value), ft_putstr_fd(EMSG_MAL, 2), NULL);
	ft_strcpy(tmp, new_str);
	free(new_str);
	new_str = tmp;
	ft_strlcat(new_str, h->var_value, ns_size + ft_strlen(h->var_value) + 1);
	h->i = h->orig_i + ft_strlen(h->var_value);
	if (needs_exp && ft_has_word(h->var_value))
		*needs_exp = 1;
	free(h->var_value);
	return (new_str);
}

char	*ft_handle_dollar_question(char *n_s, int *exit_code, int *i, char *str)
{
	char	*tmp;
	char	*ecs;

	ecs = ft_itoa(*exit_code);
	tmp = malloc(sizeof(char) * (ft_strlen(n_s)
				+ ft_strlen(ecs) + ft_strlen(&str[*i + 2]) + 1));
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

char	*ft_handle_dollar_question_q(char *new_str, int *e_c, int *i, char *str)
{
	char	*tmp;
	char	*e_c_str;

	e_c_str = ft_itoa(*e_c);
	tmp = malloc(sizeof(char) * (ft_strlen(new_str)
				+ ft_strlen(e_c_str) + ft_strlen(&str[*i + 2])));
	if (!tmp || !e_c_str)
		return (ft_putstr_fd(EMSG_MAL, 2), NULL);
	ft_strcpy(tmp, new_str);
	ft_strlcat(tmp, e_c_str, ft_strlen(new_str) + ft_strlen(e_c_str) + 1);
	*i += 2;
	free(new_str);
	free(e_c_str);
	new_str = tmp;
	return (new_str);
}
