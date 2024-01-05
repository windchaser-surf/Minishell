/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 14:10:26 by rluari            #+#    #+#             */
/*   Updated: 2024/01/05 13:32:57 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	t_parser *parser_node;

	while (parser_head)
	{
		tmp = parser_head->next;
		parser_node = (t_parser *)parser_head->content;
		if (parser_node->cmd_path && ft_strcmp( parser_node->cmd_path , "BUILTIN") != 0)
			free(parser_node->cmd_path);
		ft_free_array(parser_node->cmd_args);
		free(parser_node->heredoc);
		free(parser_head->content);
		free(parser_head);
		parser_head = tmp;
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

	asd = ft_strjoin("Minishell: ", str);
	//todo: what if malloc fails?
	perror(asd);
	free(asd);
}

_Bool	ft_ends_with_slash(char *str)
{
	int	i;

	i = ft_strlen(str) - 1;
	if (str[i] == '/')
		return (1);
	return (0);
}

int	ft_dir_accession(char *cmd, t_parser **parser_n)
{
	char	*tmp;

	tmp = NULL;
	if (is_directory(cmd) == 1)
	{
		(*parser_n)->exit_code = 126;
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
	}
	else if (is_directory(cmd) == 0)
	{
		(*parser_n)->exit_code = 126;
		ft_perror_and_free(cmd);
	}
	else
	{
		tmp = ft_substr(cmd, 0, ft_strlen(cmd) - 1);
		if (tmp == NULL)
			return (-1);
		if (access(tmp, F_OK) == 0)
			(*parser_n)->exit_code = 126;
		else
			(*parser_n)->exit_code = 127;
		ft_perror_and_free(cmd);
		free(tmp);
	}
	return (1);
}

int	ft_is_path_invalid(char *cmd, t_parser **parser_n)	//it an return 1 (invalid), 0 (valid), -1 (malloc error)
{
	if (!ft_ends_with_slash(cmd))
	{
		if (access(cmd, F_OK) == 0)
			return (0);
		else
			return ((*parser_n)->exit_code = 127, ft_perror_and_free(cmd), 1);
	}
	else	//error
		return (ft_dir_accession(cmd, parser_n));	//if it's 1 we make cmd_path to be NULL, but set just exit code to not 0, if it's -1, we free the whole parsed list

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

int	get_pos_of_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

char	*ft_get_cmd_name(char *cmd_with_path)
{
	int	i;

	i = ft_strlen(cmd_with_path) - 1;
	while (i >= 0)
	{
		if (cmd_with_path[i] == '/')
			return (ft_strdup(cmd_with_path + i + 1));
		i--;
	}
	return (NULL);
}

_Bool	ft_is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	else
		return (0);
}

char	*ft_cut_until_equal(char *str)
{
	int		i;
	char	*new_str;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	new_str = ft_substr(str, 0, i);
	if (new_str == NULL)
		return (NULL);
	return (new_str);
}

char	*ft_get_env_value(t_list *env, char *var_name)
{
	int		i;
	char	*env_name;
	char	*env_var;

	i = 0;
	while (var_name[i] && var_name[i] != '=')
		i++;
	env_name = ft_substr(var_name, 0, i);
	if (env_name == NULL)
		return (NULL);
	while (env)
	{
		env_var = ft_cut_until_equal((env)->content);
		if (env_var == NULL)
			return (NULL);
		//printf("env_name: %s\n", env_name);
		if (ft_strcmp(env_name, env_var) == 0)
			return (free(env_name), free(env_var), ft_substr((env)->content, i + 1, ft_strlen((env)->content) - i - 1));
		free(env_var);
		env = (env)->next;
	}
	free(env_name);
	return (NULL);
}

char	*ft_get_path(t_list **env, char *cmd)
{
	char	*path;
	char	**path_dirs;
	char	*cmd_path;
	int		i;

	if (ft_is_builtin(cmd))
		return (cmd = "BUILTIN", cmd);
	path = ft_get_env_value(*env, "PATH");
	path_dirs = ft_split(path, ':');
	free(path);
	if (path_dirs == NULL)
		return (NULL);
	i = -1;
	while (path_dirs[++i])
	{
		cmd_path = ft_strjoin(path_dirs[i], "/");
		cmd_path = ft_strjoin_free(cmd_path, cmd);
		if (access(cmd_path, F_OK) == 0)
		{
			ft_free_array(path_dirs);
			return (cmd_path);
		}
		free(cmd_path);
	}
	return (ft_free_array(path_dirs), NULL);
}

	
//handle absoulte and relative path, jani/ls is wrong, ls is correct, /user/bin/ls is correct
//the first WORD is the program name, every other word in order vecomes a parameter
//if the command (first WORD) doesnt exist or wrong path if given by /, then set exit_error to 1


int is_directory(const char *path)
{
    struct stat path_stat;

	// Get information about the file or directory
	if (stat(path, &path_stat) == 0)
	{
		// Check if it's a directory
		if (S_ISDIR(path_stat.st_mode))
			return 1;  // It's a directory
		else
			return 0;  // It's not a directory (could be a file)
	}
	else
	{
		return(-1);
	}
}

int	ft_handle_absolute_command(t_parser **parser_node, t_lexer *lexed_item)
{
	int validity;

	validity = ft_is_path_invalid(lexed_item->word, parser_node); //1 (invalid), 0 (valid), -1 (malloc error)
	if (validity == 1)	//INvalid path
		(*parser_node)->cmd_path = NULL;
	else
	{
		(*parser_node)->cmd_path = ft_strdup(lexed_item->word);
		if ((*parser_node)->cmd_path == NULL || validity == -1) //substr OR strdup malloc failed, so we free the whole parsed list
			return (1);
	}
	(*parser_node)->cmd_args[0] = ft_get_cmd_name(lexed_item->word);
	if (ft_is_builtin((*parser_node)->cmd_args[0]))
	{
		free ((*parser_node)->cmd_path);
		(*parser_node)->cmd_path = "BUILTIN";
	}
	return (0);
}

_Bool	ft_set_exit_err_empty_arg(t_parser **parser_node, int exit_code)
{
	char	*ec;

	if ((*parser_node) == NULL)
		return (1);
	if ((*parser_node)->cmd_args == NULL)
		return (1);
	if (ft_strcmp((*parser_node)->cmd_args[0], "exit") != 0
		|| (*parser_node)->cmd_args[1] != NULL)
		return (0);
	ec = ft_itoa(exit_code);
	if (!ec)
		return (perror("Malloc failed"), 1);
	(*parser_node)->cmd_args = ft_realloc_array((*parser_node)->cmd_args, ec);
	free(ec);
	return (0);
}

_Bool	ft_handle_word(t_parser_helper *h, t_list **env_copy)
{
	if (h->prev_was_word == 0)
	{
		h->parser_n->cmd_args = (char **)malloc(sizeof(char *) * 2);
		if (h->parser_n->cmd_args == NULL)
			return (perror("Malloc failed"), 1);
		if (ft_strchr(h->lexed_i->word, '/') || h->lexed_i->word[0] == '.')// "/usr/bin/grep"
		{
			if (ft_handle_absolute_command(&(h->parser_n), h->lexed_i) == 1)	//malloc failed
				return (perror("Malloc failed"), 1);
		}
		else// "grep"
		{
			h->parser_n->cmd_path = ft_get_path(env_copy, h->lexed_i->word);
			h->parser_n->cmd_args[0] = ft_strdup(h->lexed_i->word);
		}
		if (h->parser_n->cmd_args[0] == NULL)
			return (perror("Malloc failed"), 1);
		h->parser_n->cmd_args[1] = NULL;
		h->prev_was_word = 1;
	}
	else
	{
		h->parser_n->cmd_args = ft_realloc_array(h->parser_n->cmd_args, h->lexed_i->word);
		if (h->parser_n->cmd_args == NULL)
			return (perror("Malloc failed"), 1);
	}
	return (0);
}

_Bool	ft_str_has_quote(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

int	ft_count_amount_of_quotes(char *str)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			quotes++;
		i++;
	}
	return (quotes);
}

char	*ft_just_remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = ft_count_amount_of_quotes(str);
	new_str = (char *)malloc(sizeof(char) * (ft_strlen(str) - j + 1));
	if (new_str == NULL)
		return (NULL);
	j = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

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

void	ft_handle_heredoc(t_parser **parser_node, t_lexer *lexed_item, bool *error)
{
	char	*delim;
	char	*tmp;
	//struct sigaction	sa;

	//sa.sa_handler = heredoc_sig_handler;
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
	//sigaction(SIGINT, &sa, NULL);
	while (1)
	{
		tmp = readline("> ");
		if (ft_strcmp(delim, tmp) == 0 /*|| g_running_process.while_true == 0*/)
			break ;
		(*parser_node)->heredoc = ft_strjoin_free((*parser_node)->heredoc, tmp);
		(*parser_node)->heredoc = ft_strjoin((*parser_node)->heredoc, "\n");
		free(tmp);
	}
	//sigaction(SIGINT, &sa, NULL);

	/*init_sig();
	if (g_running_process.while_true == 0)
	{
		free((*parser_node)->heredoc);
	}
	g_running_process.while_true = 1;*/
	free(delim);
	free(tmp);
}
