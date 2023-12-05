/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:41:49 by rluari            #+#    #+#             */
/*   Updated: 2023/12/05 18:12:35 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdlib.h>
# include <unistd.h>

//for readline
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# include <sys/types.h>
# include <sys/wait.h>

# include <fcntl.h>
# include <stdbool.h>

//Lexer

typedef struct t_variable {
	char	*name;
	char	*value;
	bool	is_quoted;
	char	quoted_value;
}	t_variable;

typedef enum WordTyp
{
	WORD,
	REDIRECTION,	//>
	INPUT,			//<
	DOUBLE_REDIRECTION, //>>
	HEREDOC		//<<
}	WordTyp;


typedef struct s_lexer {
	char	*word;
	WordTyp	type;			//1: word, 2: redirection, 3: input, 4: double redirection, 5: heredoc, 6: pipe
	int		exec_num;		//the number of the command in the pipe, for example, "ls | cat | wc" -> 1 for ls, 2 for cat, 3 for wc
	//struct s_lexer	*next;
} t_lexer;

//Parsing
typedef struct s_parser		//a node is piece of element that you need to pass to the execve function
{
	//struct s_parser	*next;
	char	*cmd_path;		//the path of the command, for example, "/bin/ls"
	char	**cmd_args;			//the arguments of the command, for example, "ls -l, just like in Pipex"
	char	**infile;		//double pointer because there can be multiple input files, for example, "cat < file1 < file2"
	char	**outfile;
	int		fd_in;
	int		fd_out;
	int		fd_pipe[2];		//in case there are multiple commands, we use dup2() and make the write end the fd_out
}	t_parser;


_Bool	ft_basic_error_checker(char *command);

//Felix functions

//lexer and parser

t_list	*ft_lexer(char *command);
t_list	*ft_parser(t_list *lexed_list);

//parser and lexer utils

int		ft_check_word_first_letter(char c, t_list *lexer_head);
void	ft_skip_spaces(char *str, int *i);
void	ft_skip_to_closing_quote(char *command, int *i, char close_char);
void	ft_free_lexer(t_list *lexer_head);
void	ft_print_lexer_list(t_list *list);

//parser


//exec_utils

void    close_fds(int *fds);

#endif