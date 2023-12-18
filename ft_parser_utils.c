/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 14:10:26 by rluari            #+#    #+#             */
/*   Updated: 2023/12/18 11:52:57 by rluari           ###   ########.fr       */
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

void	ft_find_cmd_path(char **cmd)
{
	if (ft_strchr(*cmd, '/') != NULL)	//it is an absolute path
	{
		if (access(*cmd, F_OK) == 0)
			return ;
		else
		{
			ft_putstr_fd("Minishell: ", 2);
			ft_putstr_fd(*cmd, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free(*cmd);
			*cmd = NULL;
		}
	}
	else	//it is a relative path
	{}
}

void	ft_handle_redirs(t_parser **parser_node, t_lexer *lexed_item, _Bool *error, WordTyp	type)
{
	char	*outfile_name;

	outfile_name = ft_strdup(lexed_item->word);
	if ((*parser_node)->fd_out != -1)	// if there is already an outfile, close it
		close ((*parser_node)->fd_out);
	if (outfile_name == NULL)
	{
		perror("Malloc failed");
		*error = 1;
		return ;
	}
	if (access(outfile_name, F_OK) != -1)	//file exists
	{
		if (type == REDIRECTION)
			(*parser_node)->fd_out = open(outfile_name, O_WRONLY | O_TRUNC);	//truncate the file
		else if (type == DOUBLE_REDIRECTION)
			(*parser_node)->fd_out = open(outfile_name, O_WRONLY | O_APPEND);	//append to the file
	}
	else
	{
		if (type == REDIRECTION)
			(*parser_node)->fd_out = open(outfile_name, O_WRONLY | O_CREAT | O_TRUNC, 644);
		else if (type == DOUBLE_REDIRECTION)
			(*parser_node)->fd_out = open(outfile_name, O_WRONLY | O_CREAT | O_APPEND, 644);
	}
	
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

int		get_pos_of_char(char *str, char c)
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
			return ft_strdup(cmd_with_path + i + 1);
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

char	*ft_get_env_value(t_list **env, char *var_name)
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
	while (*env)
	{
		env_var = ft_cut_until_equal((*env)->content);
		//printf("env_name: %s\n", env_name);
		if (ft_strcmp(env_name, env_var) == 0)
			return (free(env_name), free(env_var),ft_substr((*env)->content, i + 1, ft_strlen((*env)->content) - i - 1));
		free(env_var);
		*env = (*env)->next;
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
	{
		cmd = "BUILTIN";
		return (cmd);
	}
	path = ft_get_env_value(env, "PATH");
	if (path == NULL)
		return (NULL);
	path_dirs = ft_split(path, ':');
	if (path_dirs == NULL)
		return (NULL);
	i = 0;
	while (path_dirs[i])
	{
		cmd_path = ft_strjoin(path_dirs[i], "/");
		cmd_path = ft_strjoin_free(cmd_path, cmd);
		if (access(cmd_path, F_OK) == 0)
		{
			ft_free_array(path_dirs);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	ft_free_array(path_dirs);
	return (NULL);

}

void	ft_handle_word(t_parser **parser_node, t_lexer *lexed_item, _Bool *error, _Bool *prev_was_word, t_list **env_copy)
{
	
	//handle absoulte and relative path, jani/ls is wrong, ls is correct, /user/bin/ls is correct
	//the first WORD is the program name, every other word in order vecomes a parameter
	//if the command (first WORD) doesnt exist or wrong path if given by /, then set exit_error to 1

	if (*prev_was_word == 0)
	{
		(*parser_node)->cmd_args = (char **)malloc(sizeof(char *) * 2);
		if ((*parser_node)->cmd_args == NULL)
		{
			*error = 1;
			return ;
		}
		if (ft_strchr(lexed_item->word, '/') != NULL)	// "/usr/bin/grep"
		{
			(*parser_node)->cmd_path = ft_strdup(lexed_item->word);
			(*parser_node)->cmd_args[0] = ft_get_cmd_name(lexed_item->word);
			if (ft_is_builtin((*parser_node)->cmd_args[0]))
			{
				free ((*parser_node)->cmd_path);
				(*parser_node)->cmd_path = "BUILTIN";
			}
		}
		else	// "grep"
		{
			(*parser_node)->cmd_path = ft_get_path(env_copy, lexed_item->word);
			(*parser_node)->cmd_args[0] = ft_strdup(lexed_item->word);
		}
		
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
	//remove quotes around heredoc	
	char	*delim;
	char	*tmp;
	
	//we dont expand variables in heredoc, so "<< $test" the delimiter is "$test" and not what's in the test variable
	if (lexed_item->word[0] == '\'' || lexed_item->word[0] == '\"')
	{
		delim = ft_substr(lexed_item->word, 1, ft_strlen(lexed_item->word) - 2);
		if (delim == NULL)
		{
			*error = 1;
			return ;
		}
	}
	else
	{
		delim = ft_strdup(lexed_item->word);
	}
	while (1)
	{
		tmp = readline("> ");
		if (ft_strcmp(delim, tmp) == 0)
			break ;
		
		(*parser_node)->heredoc = ft_strjoin_free((*parser_node)->heredoc, tmp);
		(*parser_node)->heredoc = ft_strjoin((*parser_node)->heredoc, "\n");
		free(tmp);
	}
	free(delim);
	free(tmp);
}