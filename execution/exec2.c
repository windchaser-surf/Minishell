/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:29:47 by fwechsle          #+#    #+#             */
/*   Updated: 2023/12/18 09:56:58 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_2d(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	**convert_lst_to_arr(t_list *env_copy)
{
	char **arr;
	int i;
	int lstsize;

	lstsize = ft_lstsize(env_copy);
	arr = (char **)malloc(sizeof(char *) * ((lstsize) + 1));
	if (arr == NULL)
		return (NULL);    //Error code malloc error!!!
	i = 0;
	while (env_copy)
	{
		if (env_copy->content)
			arr[i] = ft_strdup(env_copy->content);
		if (arr[i] == NULL)
			return (NULL); // Error code malloc error!!! 
		i++;
		env_copy = env_copy->next; 
	}
	arr[i] = NULL;
	return(arr);
}

void	execution(t_parser *command, t_list **env_copy)
{
	char	**envp;
	if (access(command->cmd_path, X_OK) != 0)
		exit(127);
	envp = convert_lst_to_arr(*env_copy);
	if (execve(command->cmd_path, command->cmd_args, envp) == -1)
	{
		free_2d(envp); //check where to free? 
		exit(1);
	}
	
}
