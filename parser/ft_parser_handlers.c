/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 14:10:26 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 18:23:18 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>

/*void	heredoc_sig_handler(int sig)
{
	if(sig == SIGINT)
	{
		g_running_process.while_true = 0;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();

	}
}*/

void	ft_init_expander_helper_nulls(t_expander_helper *h, t_list **env_copy)
{
	h->list_head = NULL;
	h->current_node = h->list_head;
	h->env_copy = env_copy;
	h->i = 0;
	h->orig_i = 0;
	h->vns = 0;
	h->var_value = NULL;
	h->needs_expansion = 0;
}

char	*ft_expand_inline(char *str, t_list **env_copy, _Bool had_quotes)
{
	t_expander_helper	h;
	char				*new_str;
	int					j;

	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (new_str == NULL)
		return (NULL);
	new_str[0] = '\0';
	ft_init_expander_helper_nulls(&h, env_copy);
	j = 0;
	while (str[h.i])
	{
		if (str[h.i] == '$' && !ft_is_var_char(str[h.i + 1]) && had_quotes)
		{
			new_str = ft_expand_variable(new_str, &h, 0, str);
			h.i = h.orig_i + h.vns;
			j = ft_strlen(new_str);
		}
		else
		{
			new_str[j++] = str[(h.i)++];
			new_str[j] = '\0';
		}
	}
	free(h.var_value);
	free(str);
	return (new_str);
}

void	ft_handle_heredoc(t_parser **parser_node, t_lexer *lexed_item, bool *error, t_list **env_copy)
{
	char	*delim;
	char	*tmp;

	//we dont expand variables in heredoc, so "<< $test" the delimiter is "$test" and not what's in the test variable
	if ((*parser_node)->fd_in != -1)
	{
		close ((*parser_node)->fd_in);
		(*parser_node)->fd_in = -1;
	}
	if (ft_str_has_quote(lexed_item->word))
		delim = ft_just_remove_quotes(lexed_item->word);
	else
		delim = ft_strdup(lexed_item->word);
	if ((*parser_node)->heredoc != NULL)
	{
		free((*parser_node)->heredoc);
		(*parser_node)->heredoc = NULL;
	}
	(*parser_node)->heredoc = ft_strdup("");
	if (delim == NULL || !(*parser_node)->heredoc)
	{
		*error = 1;
		return ;
	}
	tmp = NULL;
	ft_init_signals(HEREDOC_INP);
	while (1)
	{
		if ((*parser_node)->heredoc_tmp != NULL)
		{
			free((*parser_node)->heredoc);
			(*parser_node)->heredoc = ft_strdup((*parser_node)->heredoc_tmp);
			break ;
		}
		tmp = readline("> ");
		if (tmp == NULL || g_sig == CNTRL_C || ft_strcmp(delim, tmp) == 0)
			break ;
		(*parser_node)->heredoc = ft_strjoin_free((*parser_node)->heredoc, tmp);
		(*parser_node)->heredoc = ft_strjoin_free((*parser_node)->heredoc, "\n");
		free(tmp);
	}
	ft_init_signals(NOT_INPUT);
	if ((*parser_node)->heredoc && ft_strchr((*parser_node)->heredoc, '$'))
		(*parser_node)->heredoc = ft_expand_inline((*parser_node)->heredoc, env_copy, !ft_str_has_quote(lexed_item->word));
	if ((*parser_node)->heredoc != NULL)
	{
		free((*parser_node)->heredoc_tmp);
		(*parser_node)->heredoc_tmp = ft_strdup((*parser_node)->heredoc);
		if ((*parser_node)->heredoc_tmp == NULL)
			*error = 1;
	}
	else
		*error = 1;
	free(delim);
	free(tmp);
}

_Bool	ft_handle_word(t_parser_helper *h, t_list **env_copy)
{
	if (h->prev_was_word == 0)
	{
		h->parser_n->cmd_args = (char **)malloc(sizeof(char *) * 2);
		if (h->parser_n->cmd_args == NULL)
			return (ft_putstr_fd(EMSG_MAL, 2), ft_free_parser_node(&h->parser_n), 1);
		if (ft_strchr(h->lexed_i->word, '/') || (h->lexed_i->word[0] == '.' && ft_strchr(h->lexed_i->word, '/')))
		{
			if (ft_handle_absolute_command(&(h->parser_n), h->lexed_i) == 1)
				return (perror("Malloc failed\n"), ft_free_parser_node(&h->parser_n), 1);
		}
		else
		{
			h->parser_n->cmd_path = ft_get_path(env_copy, h->lexed_i->word, &h->parser_n);
			h->parser_n->cmd_args[0] = ft_strdup(h->lexed_i->word);
		}
		if (h->parser_n->cmd_args[0] == NULL)
			return (perror("Malloc failed\n"), 1);
		h->parser_n->cmd_args[1] = NULL;
		h->prev_was_word = 1;
	}
	else
	{
		h->parser_n->cmd_args = ft_realloc_array(h->parser_n->cmd_args, h->lexed_i->word);
		if (h->parser_n->cmd_args == NULL)
			return (ft_putstr_fd("Malloc failed\n", 2), ft_free_parser_node(&h->parser_n), 1);
	}
	return (0);
}

void	ft_handle_redirs(t_parser **p_n, t_lexer *l_i, WordTyp type)
{
	if ((*p_n)->fd_out != -1)
		close ((*p_n)->fd_out);
	if (access(l_i->word, F_OK) != -1)
	{
		if (type == REDIR)
			(*p_n)->fd_out = open(l_i->word, O_WRONLY | O_TRUNC);
		else if (type == D_REDIR)
			(*p_n)->fd_out = open(l_i->word, O_WRONLY | O_APPEND);
	}
	else
	{
		if (type == REDIR)
			(*p_n)->fd_out = open(l_i->word, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (type == D_REDIR)
			(*p_n)->fd_out = open(l_i->word, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	if ((*p_n)->fd_out == -1)
	{
		(*p_n)->exit_code = 1;
		ft_perror_and_free(l_i->word);
	}
}

void	ft_handle_input(t_parser **parser_node, t_lexer *lexed_item, _Bool *error)
{
	char	*infile_name;

	infile_name = ft_strdup(lexed_item->word);
	if (infile_name == NULL)
	{
		ft_free_parser_node(parser_node);
		*error = 1;
		return ;
	}
	if ((*parser_node)->heredoc)
	{
		free((*parser_node)->heredoc);
		(*parser_node)->heredoc = NULL;
	}
	if ((*parser_node)->fd_in != -1)
		close ((*parser_node)->fd_in);
	(*parser_node)->fd_in = open(infile_name, O_RDONLY);
	if ((*parser_node)->fd_in == -1)
	{
		(*parser_node)->exit_code = 1;
		ft_perror_and_free(infile_name);
	}
	free(infile_name);
}