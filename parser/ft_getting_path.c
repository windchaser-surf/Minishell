/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getting_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 12:39:35 by rluari            #+#    #+#             */
/*   Updated: 2024/01/06 12:51:01 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*ft_get_path(t_list **env, char *cmd, t_parser **parser_node)
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
	if (path_dirs == NULL || ft_cmd_is_dot(cmd, parser_node))
		return (NULL);
	i = -1;
	while (path_dirs[++i])
	{
		cmd_path = ft_strjoin(path_dirs[i], "/");
		cmd_path = ft_strjoin_free(cmd_path, cmd);
		if (access(cmd_path, F_OK) == 0 )
		{
			ft_free_array(path_dirs);
			return (cmd_path);
		}
		free(cmd_path);
	}
	return (ft_free_array(path_dirs), NULL);
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