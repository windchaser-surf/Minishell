/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 14:10:26 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 21:46:11 by fwechsle         ###   ########.fr       */
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

void	ft_checking_for_heredoc(t_parser **parser_node, t_lexer *lexed_item, \
	char **delim)
{
	if ((*parser_node)->fd_in != -1)
	{
		close ((*parser_node)->fd_in);
		(*parser_node)->fd_in = -1;
	}
	if (ft_str_has_quote(lexed_item->word))
		*delim = ft_just_remove_quotes(lexed_item->word);
	else
		*delim = ft_strdup(lexed_item->word);
	if ((*parser_node)->heredoc != NULL)
	{
		free((*parser_node)->heredoc);
		(*parser_node)->heredoc = NULL;
	}
	(*parser_node)->heredoc = ft_strdup("");
}

void	ft_handle_heredoc(t_parser **parser_node, t_lexer *lexed_item, \
	bool *error, t_list **env_copy)
{
	char	*delim;

	ft_checking_for_heredoc(parser_node, lexed_item, &delim);
	if (delim == NULL || !(*parser_node)->heredoc)
	{
		*error = 1;
		return ;
	}
	ft_get_heredoc(parser_node, delim);
	if ((*parser_node)->heredoc && ft_strchr((*parser_node)->heredoc, '$'))
		(*parser_node)->heredoc = ft_expand_inline((*parser_node)->heredoc, \
			env_copy, !ft_str_has_quote(lexed_item->word));
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
}

_Bool	ft_handle_word(t_parser_h *h, t_list **env_copy)
{
	if (h->prev_was_word == 0)
	{
		if (ft_checking_for_handle_word(h, env_copy) == 1)
			return (1);
	}
	else
	{
		h->parser_n->cmd_args = ft_realloc_array(h->parser_n->cmd_args, \
			h->lexed_i->word);
		if (h->parser_n->cmd_args == NULL)
			return (ft_putstr_fd(EMSG_MAL, 2), \
				ft_free_p_node(&h->parser_n), 1);
	}
	return (0);
}

void	ft_handle_redirs(t_parser **p_n, t_lexer *l_i, t_WordTyp type)
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
			(*p_n)->fd_out = \
				open(l_i->word, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (type == D_REDIR)
			(*p_n)->fd_out = \
				open(l_i->word, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	if ((*p_n)->fd_out == -1)
	{
		(*p_n)->exit_code = 1;
		ft_perror_and_free(l_i->word);
	}
}

void	ft_handle_input(t_parser **parser_node, t_lexer *lexed_item, \
	_Bool *error)
{
	char	*infile_name;

	infile_name = ft_strdup(lexed_item->word);
	if (infile_name == NULL)
	{
		ft_free_p_node(parser_node);
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
