/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:29:47 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 18:18:01 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	n_child_process(t_parser *command, t_list **env_copy, t_pipex *data, \
	t_list *tokens)
{
	data->pid[data->n2] = fork();
	if (data->pid[data->n2] == -1)
		return (perror("fork: "), EXIT_FAILURE);
	if (data->pid[data->n2] == 0)
	{
		ft_init_signals(CHILD);
		dup_input(command, data, tokens, env_copy);
		dup_output(command, data, tokens, env_copy);
		ft_pipe_closer(data);
		if (command->cmd_path == NULL)
			cmd_not_found(command, tokens, data, env_copy);
		else if (!ft_strncmp(command->cmd_path, "BUILTIN", 8))
		{
			data->exit_code = run_builtins(command, env_copy, \
			data->exit_code, 1);
			ft_free_child(data, tokens, env_copy);
			exit (data->exit_code);
		}
		else
			execution(command, *env_copy);
	}
	else
		ft_file_closer_single(command);
	return (EXIT_SUCCESS);
}

int	create_pipes(t_pipex *data)
{
	int	i;
	int	del;

	i = 0;
	while (i < data->nbr_p)
	{
		if (pipe(data->p + 2 * i) == -1)
		{
			perror("pipe: ");
			del = i * 2;
			while (del > 0)
			{
				del--;
				close (data->p[del]);
			}
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	preperation_for_child(t_pipex *data, t_list *tokens)
{
	data->nbr_p = ft_lstsize(tokens) - 1;
	data->p = (int *)malloc(sizeof(int) * (2 * data->nbr_p));
	if (data->p == NULL)
	{
		perror("malloc: ");
		return (MALLOC_ERR);
	}
	if (create_pipes(data))
	{
		free(data->p);
		return (EXIT_FAILURE);
	}
	data->pid = (int *)malloc(sizeof(int) * ft_lstsize(tokens));
	if (data->pid == NULL)
	{
		perror("malloc: ");
		ft_pipe_closer(data);
		free(data->p);
		return (MALLOC_ERR);
	}
	return (EXIT_SUCCESS);
}

int	waiting_for_childs(t_pipex *data, int n)
{
	int	status;

	status = 0;
	data->n = n;
	ft_pipe_closer(data);
	n = 0;
	while (n < data->n)
	{
		waitpid(data->pid[n], &status, 0);
		n++;
	}
	status = WEXITSTATUS(status);
	free(data->pid);
	free(data->p);
	return (status);
}

int	n_execution(t_list *tokens, t_list **env_copy, int exit_code)
{
	t_pipex	data;
	t_list	*tmp;

	tmp = tokens;
	data.n2 = 0;
	data.exit_code = exit_code;
	if (preperation_for_child(&data, tokens))
		return (EXIT_FAILURE);
	while (tmp != NULL)
	{
		if (n_child_process((t_parser *)(tmp->content), env_copy, \
			&data, tokens))
			return (EXIT_FAILURE);
		data.n2++;
		tmp = tmp->next;
	}
	return (waiting_for_childs(&data, data.n2));
}
