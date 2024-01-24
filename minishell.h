/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 21:29:14 by rluari            #+#    #+#             */
/*   Updated: 2024/01/24 14:36:44 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>

//for readline
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# include <sys/types.h>
# include <sys/wait.h>

# include <fcntl.h>
# include <stdbool.h>

//for checking paths
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>

//inclide other header files

# define MALLOC_ERR 1 
# define CMD_NOT_FOUND 127

# define EMSG_SYN "Minishell: syntax error near unexpected token `"
# define EMSG_MAL "Minishell: Malloc error\n"

typedef struct s_variable
{
	char	*name;
	char	*value;
	_Bool	is_quoted;
	char	quoted_value;
}	t_variable;

typedef enum WordTyp
{
	WORD = 1,
	REDIR = 2,	//>
	INFILE = 3,	//<
	D_REDIR = 4, //>>
	HEREDOC	= 5//<<
}	t_WordTyp;

typedef enum SigTyp
{
	INPUT = 1,
	NOT_INPUT = 2,
	CHILD = 3,
	HEREDOC_INP = 4,
}	t_SigTyp;

enum	e_sig
{
	CNTRL_C = 1,
	CNTRL_D = 2,
};

typedef struct s_pipex
{
	int	*pid;
	int	n;
	int	n2;
	int	nbr_p;
	int	*p;
	int	exit_code;
}			t_pipex;

typedef struct s_lexer
{
	char		*word;
	t_WordTyp	type;
	int			exec_num;
	_Bool		wasnt_empty_var;
}	t_lexer;

typedef struct s_parser
{
	char	*cmd_path;
	char	**cmd_args;
	char	*heredoc;
	char	*heredoc_tmp;
	int		exit_code;
	int		fd_in;
	int		fd_out;
	int		*fd_pipe;
}	t_parser;

//Syntax checking
_Bool	ft_basic_error_checker(char **command, int *error_code);
int		ft_ends_with_spec(char *command, int *error_code);
_Bool	ft_unmatched_quotes(char *command, int *error_code);
_Bool	ft_emptyness_in_cmd(char *cmd, int i, int *error_code, \
	_Bool prev_was_pipe);

typedef struct s_main_helper
{
	char		*command;
	t_list		*lexed_list;
	t_list		*env_copy;
	t_list		*parsed_list;
	int			exit_code;
	t_SigTyp	sig_mode;
}			t_main_helper;

//BUILTIN
//cd.c 
int		ft_pwd_builtin(void);
void	ft_set_old_env(t_list **env_copy, char *old_pwd);
void	ft_set_new_env(t_list **env_copy);
int		ft_change_to_home(t_list **env_copy);
int		ft_change_to_previous(t_list **env_copy);

//cd2.c
int		cd_builtin(char **cmd, t_list **env_copy);
int		ft_change_to_dir(char *cmd, t_list **env_copy);

//echo.c
int		ft_echo_builtin(char **arg);

//env.c
int		init_env(char **env, t_list **env_copy);
int		print_env(t_list *env_copy);
void	del(void *content);

//export.c
char	**sort_arr(char **arr, int size);
int		print_alpha(char **arr);
char	**ft_copy_lst_to_arr(t_list *env_copy, char **arr);
int		print_export(t_list *env_copy);
int		err_msg_not_valid(void);

//export2.c
int		valid_new_variable(char *cmd);
int		check_exist(char *var, t_list *env_copy);
int		set_value_env(char *cmd, t_list *env_copy);
int		export_builtin(char **cmd, t_list **env_copy);

//unset.c
int		builtin_unset(t_list **env_copy, char **var, int i);

//exec_main.c
void	execution_main(t_list *tokens, t_list **env_copy);
char	**convert_lst_to_arr(t_list *env_copy);
void	execution(t_parser *command, t_list *env_copy);
void	free_2d(char **str);

//exec_utils.c
void	dup_heredoc(t_parser *command);
void	close_fds(int *fds);

//exec1.c
void	ft_file_closer_single(t_parser *command);
void	child_process(t_parser *command, t_list **env_copy);
void	cmd_path_null(t_parser *command);
void	one_execution(t_parser *command, t_list **env_copy, \
	t_list *tokens);

//exec1_1.c
void	exec_builtins(t_parser *command, t_list **env_copy, \
	t_list *tokens);

//exec2.c
int		n_child_process(t_parser *command, t_list **env_copy, t_pipex *data, \
	t_list *tokens);
int		create_pipes(t_pipex *data);
int		preperation_for_child(t_pipex *data, t_list *tokens);
void	waiting_for_childs(t_pipex *data, int n);
void	n_execution(t_list *tokens, t_list **env_copy);

//exec2_2.c
void	dup_output(t_parser *command, t_pipex *data, t_list *tokens, \
	t_list **env_copy);
void	ft_free_child(t_pipex *data, t_list *tokens, t_list **env_copy);
void	dup_input(t_parser *command, t_pipex *data, t_list *tokens, \
	t_list **env_copy);
void	error_closing(t_parser *command, t_pipex *data, t_list *tokens, \
	t_list **env_copy);
void	ft_pipe_closer(t_pipex *data);

//exec2_3.c
void	cmd_not_found(t_parser *command, t_list *tokens, t_pipex *data, \
	t_list **env_copy);

//check_builtin.c
int		check_builtin(char *str);
int		run_builtins(t_parser *command, t_list **env_copy, int error_code, \
	int pid_check);
void	run_builtins_parent(t_parser *command, t_list **env_copy, \
	t_list *tokens);

//exit.c
long	ft_atoi_long(const char *str);
int		check_for_number(char *arg);
int		exit_too_many(int pid_check);
int		exit_not_numeric(int pid_check, t_list *tokens, t_list **env_copy);
int		exit_with_number(int pid_check, char *numb, t_list *tokens, \
	t_list **env_copy);

//exit2.c
int		builtin_exit(char **arg, int exit_code, int pid_check);
int		builtin_exit_parent(char **arg, t_list *tokens, \
	t_list **env_copy);

//free. currently in cd file
void	free_2d(char **str);

//BUILTIN END

//Signals

extern int	g_ec;

void	ft_init_signals(t_SigTyp sig_situation);

//Lexer and it's utils
t_list	*ft_lexer(char *command, int *exit_code);

typedef struct s_lexer_helper
{
	t_list	*list_head;
	int		start;
	int		i;
	int		prev_wt;
	int		exec_num;
}	t_lexer_helper;

int		ft_check_word_first_letter(char c, t_list *lexer_head, int *exit_code);
void	ft_skip_spaces(char *str, int *i);
void	ft_skip_to_closing_quote(char *command, int *i, char close_char);
void	ft_print_lexer_list(t_list *list);

_Bool	ft_handle_lexer_word(t_lexer_helper *helper, char *command, \
	int *exit_code);
_Bool	ft_handle_lexer_redir(t_lexer_helper *helper, char *command, \
	int *exit_code);
_Bool	ft_handle_lexer_input(t_lexer_helper *helper, char *command, \
	int *exit_code);
_Bool	ft_lexer_while(t_lexer_helper *helper, char *command, \
	int *exit_code);
_Bool	ft_handle_lexer_new_command(t_lexer_helper *h, char *cmd, \
	int *exit_code);

_Bool	ft_make_lnode(t_lexer_helper *helper, char *command);
void	ft_init_lexer_helper(t_lexer_helper *helper, char *command, \
	int exec_num);
//int		ft_check_for_empty_command(t_list *list_head);

void	ft_free_lexer(t_list *lexer_head);
void	ft_free_lexer_node(t_list *lexer_head);
void	ft_free_array(char **arr);

//Expander

typedef struct s_expander_helper
{
	t_list	*list_head;
	t_list	*current_node;
	//t_lexer	*curr_cont;
	t_list	**env_copy;
	int		i;
	int		orig_i;
	int		vns;
	char	*var_value;
	_Bool	needs_expansion;
}	t_ex_h;

//ft_expander
void	init_expander_helper(t_ex_h *h, t_list **lexed_list, \
	t_list **env_copy);
char	*ft_checker_for_expander(t_ex_h *h, int exit_code, \
	char *str);
t_list	*ft_expander(t_list **lexed_list, t_list **env_copy, int exit_code);

//ft_expander_utils.c
char	*ft_remove_quote(char *str, int *i, char c);
char	*ft_make_new_str_for_expanstion(int *i, char *str, int *j);
void	ft_copy_whats_after_the_quote(int *i, int *j, char *str, char *new_str);
t_ex_h	*ft_checker_expand_dqoute(t_ex_h *h, \
	char **new_str, char *str);
char	*ft_expand_dquote(t_ex_h *h, int exit_code);

//ft_expander_utils2.c
t_list	*ft_lexer_but_with_words_and_one_cmd(char *command, int cmd_num);
char	*ft_concat_rest(char *str, t_ex_h *h, char *new_str, \
	_Bool needs_expansion);
char	*ft_attach_beginning(char *head_node_str, char *str, \
	t_ex_h *h);
_Bool	ft_is_var_char(char c);
char	*ft_checker_for_expand_with_split(t_ex_h *h, char *new_str, \
	t_lexer *orig_lex_node);

//ft_expander_utils3.c
char	*ft_expand_with_split(t_ex_h *h, int *exit_code);
int		ft_get_number_of_nodes_in_cmd(t_list *lexed_l, int i);
void	ft_helper_put_node_to_end_of_cmd(t_list *t_b_m, t_list **first_in_cmd, \
	t_list **orig, t_list *prev);
void	ft_put_node_to_end_of_cmd(t_list **orig, t_list **first_in_cmd, \
t_list *t_b_m);
void	ft_rearrange_lexed_list(t_list **lexed_l, int i, t_ex_h *h);

int		ft_get_var_name_size(char *str, int *i);
char	*ft_get_var_value(char *var_name, t_list **env_copy, _Bool need_free);
char	*ft_expand_variable(char *new_str, t_ex_h *h, \
	_Bool *needs_expansion, char *str);
char	*ft_get_var_name(char *str);
char	*ft_handle_dollar_question(char *new_str, int *exit_code, int *i, \
	char *str);
char	*ft_handle_dollar_question_q(char *new_str, int *exit_code, int *i, \
	char *str);

void	p_amb_re(char *var_name);

char	*ft_insert_new_lexed_nodes(t_list *new_nodes_head, \
	t_ex_h *h);
//void	ft_rearrange_lexed_list(t_list **lexed_l, int i);
_Bool	ft_add_back_to_new_ll(t_list **new_lexed_list, t_list *lexed_list, \
	_Bool num);

//Parser and it's utils
t_list	*ft_parser(t_list *lexed_list, t_list **env_copy);

typedef struct s_parser_helper
{
	t_list		*p_list_head;
	t_list		*p_new_node;
	t_parser	*parser_n;
	t_lexer		*lexed_i;
	int			i_cmd;
	_Bool		error;
	_Bool		prev_was_word;
}	t_parser_h;

void	ft_init_parser_node(t_parser **parser_node);
void	ft_handle_redirs(t_parser **parser_node, t_lexer *lexed_item, \
	t_WordTyp type);
void	ft_handle_input(t_parser **parser_node, t_lexer *lexed_item, \
	_Bool *error);
void	ft_handle_heredoc(t_parser **parser_node, t_lexer *lexed_item, \
	_Bool *error, t_list **env_copy);
_Bool	ft_handle_word(t_parser_h *helper, t_list **env_copy);
void	ft_init_expander_helper_nulls(t_ex_h *h, t_list **env_copy);
char	*ft_free_for_expand_inline(char *new_str, char *str, \
	t_ex_h *h);
char	*ft_expand_inline(char *str, t_list **env_copy, _Bool had_quotes);
void	ft_get_heredoc(t_parser **parser_node, char *delim);
int		ft_checking_for_handle_word(t_parser_h *h, t_list **env_copy);
void	ft_init_parser_helper_struct(t_parser_h *helper);
_Bool	ft_is_empty_lexed_lode(t_list **lexed_list);
void	ft_free_parser_makefiles(t_list *p_list_head);
_Bool	ft_make_command(t_parser_h *h, _Bool last);
void	ft_free_parser(t_list *parser_head);
void	ft_free_p_node(t_parser **parser_node);
char	**ft_realloc_array(char **array, char *new_item);
void	ft_perror_and_free(char *str);

int		ft_dir_accession(char *cmd, t_parser **parser_n);
int		ft_is_path_invalid(char *cmd, t_parser **parser_n);
int		get_pos_of_char(char *str, char c);
char	*ft_get_cmd_name(char *cmd_with_path);
_Bool	ft_is_builtin(char *cmd);
char	*ft_cut_until_equal(char *str);
char	*ft_get_env_value(t_list *env, char *var_name);
_Bool	ft_cmd_is_dot(char *cmd, t_parser **parser_node);
char	*ft_get_path(t_list **env, char *cmd, t_parser **parser_node);
int		is_directory(const char *path);
int		ft_handle_absolute_command(t_parser **parser_node, t_lexer *lexed_item);
_Bool	ft_str_has_quote(char *str);
int		ft_count_amount_of_quotes(char *str);
char	*ft_just_remove_quotes(char *str);
void	ft_sighandle_child(int sig);
void	ft_sighandle_heredoc(int sig);

#endif
