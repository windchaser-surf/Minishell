/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 14:10:26 by rluari            #+#    #+#             */
/*   Updated: 2023/12/14 13:03:31 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	ft_init_parser_node(t_parser **parser_node)
{
	(*parser_node)->cmd_path = NULL;
	(*parser_node)->cmd_args = NULL;
	(*parser_node)->fd_in = -1;
	(*parser_node)->fd_out = -1;
	(*parser_node)->exit_code = 0;
	(*parser_node)->heredoc = NULL;
}

void	ft_free_parser(t_list *parser_head)
{
	t_list	*tmp;

	while (parser_head)
	{
		tmp = parser_head->next;
		free(((t_parser *)parser_head->content)->cmd_path);
		free(((t_parser *)parser_head->content)->heredoc);
		ft_free_array(((t_parser *)parser_head->content)->cmd_args);
		//free(((t_parser *)parser_head->content)->infile);
		//free(((t_parser *)parser_head->content)->outfile->outfile_str);  ---  iterate through the outfile ll and free each char *
		free(parser_head->content);
		free(parser_head);
		parser_head = tmp;
		if (parser_head == NULL)
			break ;
	}
}

char	**ft_realloc_array(char **outfiles, char *new_item)
{
	int		i;
	char	**new_array;

	i = 0;
	while (outfiles[i])
		i++;
	new_array = (char **)malloc(sizeof(char *) * (i + 2));
	if (new_array == NULL)
		return (NULL);
	i = 0;
	while (outfiles[i])
	{
		new_array[i] = ft_strdup(outfiles[i]);
		if (new_array[i] == NULL)
			return (NULL);
		i++;
	}
	new_array[i] = ft_strdup(new_item);
	if (new_array[i] == NULL)
		return (NULL);
	new_array[i + 1] = NULL;
	ft_free_array(outfiles);
	return (new_array);
}

void	ft_perror_and_free(char *str)
{
	char	*asd;

	asd = ft_strjoin("Minishell", str);
	//todo: what if malloc fails?
	perror(asd);
	free(asd);
}

void	ft_handle_redirs(t_parser **parser_node, t_lexer *lexed_item, _Bool *error, WordTyp	type)
{
	char	*outfile_name;

	outfile_name = ft_strdup(lexed_item->word);
	if ((*parser_node)->fd_out != -1)	// if there is already an outfile, close it
		close ((*parser_node)->fd_out);
	if (outfile_name == NULL)
	{
		*error = 1;
		return ;
	}
	if (type == REDIRECTION)
		(*parser_node)->fd_out = open(outfile_name, O_WRONLY | O_TRUNC);
	else if (type == DOUBLE_REDIRECTION)
		(*parser_node)->fd_out = open(outfile_name, O_WRONLY | O_APPEND);
	if ((*parser_node)->fd_out == -1)
	{
		(*parser_node)->exit_code = 1;
		ft_perror_and_free(outfile_name);
	}
	free(outfile_name);

}

void	ft_handle_input(t_parser **parser_node, t_lexer *lexed_item, _Bool *error)
{
	char	*infile_name;

	infile_name = ft_strdup(lexed_item->word);
	if (infile_name == NULL)
	{
		*error = 1;
		return ;
	}
	if ((*parser_node)->fd_in != -1)	// if there is already an infile, close it
		close ((*parser_node)->fd_in);
	(*parser_node)->fd_in = open(infile_name, O_RDONLY);
	if ((*parser_node)->fd_in == -1)
	{
		(*parser_node)->exit_code = 1;
		ft_perror_and_free(infile_name);
	}
	free(infile_name);
}

void	ft_handle_word(t_parser **parser_node, t_lexer *lexed_item, _Bool *error, _Bool *prev_was_word)
{
	//handle absoulte and relative path, jani/ls is wrong, ls is correct, /user/bin/ls is correct
	//the first WORD is the program name, every other word in order vecomes a parameter
	//if the command (first WORD) doesnt exist or wrong path if given by /, then set exit_error to 1
	if (*prev_was_word == 0)
	{
		(*parser_node)->cmd_path = ft_strdup(lexed_item->word);
		if ((*parser_node)->cmd_path == NULL)
		{
			*error = 1;
			return ;
		}
		(*parser_node)->cmd_args = (char **)malloc(sizeof(char *) * 2);
		if ((*parser_node)->cmd_args == NULL)
		{
			*error = 1;
			return ;
		}
		(*parser_node)->cmd_args[0] = ft_strdup(lexed_item->word);
		if ((*parser_node)->cmd_args[0] == NULL)
		{
			*error = 1;
			return ;
		}
		(*parser_node)->cmd_args[1] = NULL;
		*prev_was_word = 1;
	}
	else
	{
		(*parser_node)->cmd_args = ft_realloc_array((*parser_node)->cmd_args, lexed_item->word);
		if ((*parser_node)->cmd_args == NULL)
		{
			*error = 1;
			return ;
		}

	}
}

void	ft_handle_heredoc(t_parser **parser_node, t_lexer *lexed_item, bool *error)
{
	
	(void)parser_node;
	(void)lexed_item;
	(void)error;
}