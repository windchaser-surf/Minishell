/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 14:10:26 by rluari            #+#    #+#             */
/*   Updated: 2023/12/13 12:23:07 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_parser_node(t_parser **parser_node)
{
	(*parser_node)->cmd_path = NULL;
	(*parser_node)->cmd_args = NULL;
	(*parser_node)->infile = NULL;
	(*parser_node)->outfile = NULL;
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
		ft_free_array(((t_parser *)parser_head->content)->infile);
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

void	ft_handle_redirs(t_parser **parser_node, t_lexer *lexed_item, int *error, t_list **outfiles)
{
	if ((*parser_node)->outfile)	//if there is no outfile yet, create one
	{
		t_out	*new_outfile_node;

		new_outfile_node = (t_out *)malloc(sizeof(t_out));
		if (new_outfile_node == NULL)
		{
			*error = 1;
			return ;
		}
		new_outfile_node->outfile_str = ft_strdup(lexed_item->word);
		new_outfile_node->type = lexed_item->type;
		
		//put into outfiles linkeldist the new_outfile_node
		
		
		if (access(new_outfile_node->outfile_str, F_OK) == 0) {
        // The file exists, check for writability and truncatability
			if (access(new_outfile_node->outfile_str, W_OK) == 0) {

				// Check if the file is truncatable (open with O_WRONLY and O_TRUNC flags)
				int file_descriptor = open(new_outfile_node->outfile_str, O_WRONLY | O_TRUNC);
				if (file_descriptor == -1)
				{
					(*parser_node)->exit_code = 1;
					errno = EINVAL;
					ft_perror_and_free(new_outfile_node->outfile_str);

				}
				else
					close(file_descriptor);
			}
			else
			{
				// The file is not writable
				(*parser_node)->exit_code = 1;
				errno = EACCES;
				perror(new_outfile_node->outfile_str);
			}
		}
		else
		{
			//create empty file with that name
		}
	}
}

void	ft_handle_input(t_parser **parser_node, t_lexer *lexed_item, int *error)
{
	if ((*parser_node)->infile == NULL)	//if there is no infile yet, create one
	{
		(*parser_node)->infile = (char **)malloc(sizeof(char *) * 2);
		if ((*parser_node)->infile == NULL)
		{
			*error = 1;
			return ;
		}
		(*parser_node)->infile[0] = ft_strdup(lexed_item->word);
		(*parser_node)->infile[1] = NULL;
	}
	else //if there is already an infile, realloc it
	{
		(*parser_node)->infile = ft_realloc_array((*parser_node)->infile, lexed_item->word);	//lexed_item->word this will be freed after the parsing has finished
		if ((*parser_node)->infile == NULL)
		{
			*error = 1;
			return ;
		}
	}
	if (access(lexed_item->word, F_OK) == 0)
	{
		// The file exists, check for readability
		if (access(lexed_item->word, R_OK) != 0)
		{
			errno = EACCES;
			ft_perror_and_free(lexed_item->word);
			(*parser_node)->exit_code = 1;

		}
		//	printf("Minishell: %s: Permission denied\n", lexed_item->word);
	}
	else
	{
		errno = ENOENT;
		ft_perror_and_free(lexed_item->word);
		(*parser_node)->exit_code = 1;
	}
	/*if (access(lexed_item->word, F_OK) != 0 || access(lexed_item->word, R_OK) != 0)
		(*parser_node)->error_in_files = 1;*/
}

void	ft_handle_word(t_parser **parser_node, t_lexer *lexed_item, int *error, _Bool *prev_was_word)
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