/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 14:10:26 by rluari            #+#    #+#             */
/*   Updated: 2024/01/17 16:13:54 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	ft_handle_heredoc(t_parser **parser_node, t_lexer *lexed_item, bool *error, SigTyp *sig_mode)
{
	char	*delim;
	char	*tmp;
;
	//we dont expand variables in heredoc, so "<< $test" the delimiter is "$test" and not what's in the test variable
	if (ft_str_has_quote(lexed_item->word))
		delim = ft_just_remove_quotes(lexed_item->word);
	else
		delim = ft_strdup(lexed_item->word);
	if (delim == NULL)
	{
		*error = 1;
		return ;
	}
	if ((*parser_node)->heredoc != NULL)
		free((*parser_node)->heredoc);
	(*parser_node)->heredoc = ft_strdup("");
	ft_set_mode(sig_mode, HEREDOC_INP);
	while (1)
	{
		tmp = readline("> ");
		if (g_sig == CNTRL_C || ft_strcmp(delim, tmp) == 0)
			break ;
		(*parser_node)->heredoc = ft_strjoin_free((*parser_node)->heredoc, tmp);
		(*parser_node)->heredoc = ft_strjoin_free((*parser_node)->heredoc, "\n");
		free(tmp);
	}
	ft_set_mode(sig_mode, NOT_INPUT);
	free(delim);
	free(tmp);
}

_Bool	ft_handle_word(t_parser_helper *h, t_list **env_copy)
{
	if (h->prev_was_word == 0)
	{
		h->parser_n->cmd_args = (char **)malloc(sizeof(char *) * 2);
		if (h->parser_n->cmd_args == NULL)
			return (perror("Malloc failed"), ft_free_parser_node(h->parser_n), 1);
		if (ft_strchr(h->lexed_i->word, '/') || (h->lexed_i->word[0] == '.' && ft_strchr(h->lexed_i->word, '/')))// "/usr/bin/grep" or "./user/bin/grep"
		{
			if (ft_handle_absolute_command(&(h->parser_n), h->lexed_i) == 1)	//malloc failed
				return (perror("Malloc failed"), ft_free_parser_node(h->parser_n), 1);
		}
		else// "grep"
		{
			h->parser_n->cmd_path = ft_get_path(env_copy, h->lexed_i->word, &h->parser_n);
			h->parser_n->cmd_args[0] = ft_strdup(h->lexed_i->word);
		}
		if (h->parser_n->cmd_args[0] == NULL)
			return (perror("Malloc failed"), ft_free_parser_node(h->parser_n), 1);
		h->parser_n->cmd_args[1] = NULL;
		h->prev_was_word = 1;
	}
	else
	{
		h->parser_n->cmd_args = ft_realloc_array(h->parser_n->cmd_args, h->lexed_i->word);
		if (h->parser_n->cmd_args == NULL)
			return (perror("Malloc failed"), ft_free_parser_node(h->parser_n), 1);
	}
	return (0);
}

void	ft_handle_redirs(t_parser **parser_node, t_lexer *lexed_item, WordTyp type)
{
	if ((*parser_node)->fd_out != -1)// if there is already an outfile, close it
		close ((*parser_node)->fd_out);
	if (access(lexed_item->word, F_OK) != -1)	//file exists
	{
		if (type == REDIR)
			(*parser_node)->fd_out = open(lexed_item->word, O_WRONLY | O_TRUNC);	//truncate the file
		else if (type == D_REDIR)
			(*parser_node)->fd_out = open(lexed_item->word, O_WRONLY | O_APPEND);	//append to the file
	}
	else
	{
		if (type == REDIR)
			(*parser_node)->fd_out = open(lexed_item->word, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (type == D_REDIR)
			(*parser_node)->fd_out = open(lexed_item->word, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	if ((*parser_node)->fd_out == -1)
	{
		(*parser_node)->exit_code = 1;
		ft_perror_and_free(lexed_item->word);
	}
}

void	ft_handle_input(t_parser **parser_node, t_lexer *lexed_item, _Bool *error)
{
	char	*infile_name;

	infile_name = ft_strdup(lexed_item->word);
	if (infile_name == NULL)
	{
		free(*parser_node);
		*error = 1;
		return ;
	}
	if ((*parser_node)->fd_in != -1)// if there is already an infile, close it
		close ((*parser_node)->fd_in);
	(*parser_node)->fd_in = open(infile_name, O_RDONLY);
	if ((*parser_node)->fd_in == -1)
	{
		(*parser_node)->exit_code = 1;
		ft_perror_and_free(infile_name);
	}
	free(infile_name);
}