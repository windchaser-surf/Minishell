/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:41:49 by rluari            #+#    #+#             */
/*   Updated: 2024/01/03 20:36:41 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>

//for readline
# include <stdio.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# include <sys/types.h>
# include <sys/wait.h>

# include <fcntl.h>
# include <stdbool.h>

#define MALLOC_ERR 1 
#define CMD_NOT_FOUND 127

typedef struct t_variable
{
	char	*name;
	char	*value;
	bool	is_quoted;
	char	quoted_value;
}	t_variable;

typedef enum WordTyp
{
	WORD,
	REDIR,	//>
	INPUT,	//<
	D_REDIR, //>>
	HEREDOC	//<<
}	WordTyp;

typedef struct s_pipex
{
	int	*pid;
	int	n;
	int	nbr_p;
	int	*p;
	int exit_code;
}			t_pipex;

typedef struct s_lexer
{
	char	*word;
	WordTyp	type;			//1: word, 2: redirection, 3: input, 4: double redirection, 5: heredoc, 6: pipe
	int		exec_num;		//the number of the command in the pipe, for example, "ls | cat | wc" -> 1 for ls, 2 for cat, 3 for wc
} t_lexer;

typedef struct s_parser		//a node is piece of element that you need to pass to the execve function
{
	char	*cmd_path;		//the path of the command, for example, "/bin/ls", or BUILTIN or NULL
	char	**cmd_args;		//the arguments of the command, for example, "ls -l, just like in Pipex"
	char	*heredoc;
	int		exit_code;
	int		fd_in;
	int		fd_out;
	int		*fd_pipe;		//in case there are multiple commands, we use dup2() and make the write end the fd_out
}	t_parser;

//Syntax checking
_Bool	ft_basic_error_checker(char **command, int *error_code);
int		ft_ends_with_spec(char *command);
_Bool	ft_unmatched_quotes(char *command);
int		ft_ends_with_spec(char *command);
_Bool	ft_is_empty_command(char *command);

//BUILTIN

//cd.c 
int    cd_builtin(char **cmd, t_list **env_copy);
int	ft_pwd_builtin(void);

//echo.c
int    ft_echo_builtin(char **arg);

//env.c
int	 init_env(char **env, t_list **env_copy);
int	 print_env(t_list *env_copy);
void	del(void *content);

//export.c
int	export_builtin(char **cmd, t_list **env_copy);
int	print_export(t_list *env_copy);
int	    check_exist(char *var, t_list *env_copy);

//unset.c
int    builtin_unset(t_list **env_copy, char **var);

//exec_main.c
int    execution_main(t_list *tokens, t_list **env_copy, int exit_code);
char	**convert_lst_to_arr(t_list *env_copy);
void	execution(t_parser *command, t_list *env_copy);
void	free_2d(char **str);

//exec_utils.c
void	dup_heredoc(t_parser *command);

//exec1.c
int exec_builtins(t_parser *command, t_list **env_copy, int exit_code, t_list *tokens);
void	ft_file_closer_single(t_parser *command);
int	child_process(t_parser *command, t_list **env_copy);
int	cmd_path_NULL(t_parser *command);
int    one_execution(t_parser *command, t_list **env_copy, int exit_code);

//exec2.c
void	ft_pipe_closer(t_pipex *data);
int n_child_process(t_parser *command, t_list **env_copy, t_pipex *data, int n);
int    create_pipes(t_pipex *data);
int	n_execution(t_list *tokens, t_list **env_copy, int exit_code);


//check_builtin.c
int check_builtin(char *str);
int run_builtins(t_parser *command, t_list **env_copy, int error_code, int pid_check);
int run_builtins_parent(t_parser *command, t_list **env_copy, int error_code, t_list *tokens);


//exit.c
int    builtin_exit(char **arg, int exit_code, int pid_check);
int    builtin_exit_parent(char **arg, int exit_code, t_list *tokens);

//free. currently in cd file
void	free_2d(char **str);
//BUILTIN END

//Lexer and it's utils
t_list	*ft_lexer(char *command);

typedef struct s_lexer_helper {
	t_list	*list_head;
	int		start;
	int		i;
	int		prev_wt;
	int		exec_num;
}	t_lexer_helper;

int		ft_check_word_first_letter(char c, t_list *lexer_head);
void	ft_skip_spaces(char *str, int *i);
void	ft_skip_to_closing_quote(char *command, int *i, char close_char);
void	ft_free_lexer(t_list *lexer_head);
void	ft_print_lexer_list(t_list *list);
int		ft_check_for_empty_command(t_list *list_head);
void	ft_free_array(char **arr);


//Expander
void	ft_expander(t_list **lexed_list, t_list **env_copy, int exit_code);

char	*ft_remove_quote(char *str, int *i, char c);
char	*ft_expand_dquote(char *str, int *i, t_list **env_copy, int exit_code);
char	*ft_expand_with_split(t_list **lexed_list_head ,t_list **lexer_node, int *i, t_list **env_copy, int *exit_code);

char	*ft_get_var_value(char *var_name, char *str, int *i, t_list **env_copy);
char	*ft_expand_variable(char *new_str, int *i, char *str, t_list **env_copy);
char	*ft_get_var_name(char *str);
char	*ft_handle_dollar_question(char *new_str, int *exit_code, int *i, char *str);

//Parser and it's utils
t_list	*ft_parser(t_list *lexed_list, int *exit_code, t_list **env_copy);

typedef struct s_parser_helper {
	t_list		*list_head;
	t_list		*new_node_head;
	t_parser	*parser_n;
	t_lexer		*lexed_i;
	int			ith_command;
	_Bool		error;
	_Bool		prev_was_word;
}	t_parser_helper;

void	ft_free_parser(t_list *parser_head);
char	**ft_realloc_array(char **array, char *new_item);
void	ft_init_parser_node(t_parser **parser_node);
void	ft_handle_redirs(t_parser **parser_node, t_lexer *lexed_item, WordTyp type);
void	ft_handle_input(t_parser **parser_node, t_lexer *lexed_item, _Bool *error);
void	ft_handle_heredoc(t_parser **parser_node, t_lexer *lexed_item, _Bool *error);
_Bool	ft_handle_word(t_parser_helper *helper, t_list **env_copy);

_Bool	ft_set_exit_err_empty_arg(t_parser **parser_node, int exit_code);
int		ft_is_redirsign(char c);
char	*ft_get_env_value(t_list *env, char *var_name);
char	*ft_cut_until_equal(char *str);
void	ft_perror_and_free(char *str);

//exec_utils

void    close_fds(int *fds);

#endif
