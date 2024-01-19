/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:19:01 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 17:57:07 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execution_main(t_list *tokens, t_list **env_copy, int exit_code)
{
	int	p_nbr;

	p_nbr = ft_lstsize(tokens);
	if (p_nbr == 1)
		return (one_execution((t_parser *)(tokens->content), env_copy, \
			exit_code, tokens));
	else if (p_nbr > 1)
		return (n_execution(tokens, env_copy, exit_code));
	return (EXIT_SUCCESS);
}

char	**free_error(char **arr, int i)
{
	int	n;

	n = 0;
	while (n < i)
	{
		free(arr[n]);
		n++;
	}
	free(arr);
	return (NULL);
}

char	**convert_lst_to_arr(t_list *env_copy)
{
	char	**arr;
	int		i;
	int		lstsize;
	t_list	*tmp;

	tmp = env_copy;
	lstsize = ft_lstsize(env_copy);
	arr = (char **)malloc(sizeof(char *) * ((lstsize) + 1));
	if (arr == NULL)
		return (perror("malloc"), NULL);
	i = 0;
	while (tmp)
	{
		if (tmp->content)
		{
			arr[i] = ft_strdup(tmp->content);
			if (arr[i] == NULL)
				return (perror("malloc"), free_error(arr, i));
			i++;
		}
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	execution(t_parser *command, t_list *env_copy)
{
	char	**envp;
	t_list	*tmp;

	tmp = env_copy;
	envp = convert_lst_to_arr(tmp);
	if (envp == NULL)
		exit (MALLOC_ERR);
	if (execve(command->cmd_path, command->cmd_args, envp) == -1)
	{
		free_2d(envp);
		exit(1);
	}
}

void	free_2d(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}
