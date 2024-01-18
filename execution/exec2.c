/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:29:47 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/18 13:46:20 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pipe_closer(t_pipex *data)
{
	int	i; 

	i = 0;
	while (i < (data->nbr_p * 2))
	{
		close(data->p[i]);
		i++;
	}
	
}

void	error_closing(t_parser *command, t_pipex *data, t_list *tokens, t_list **env_copy)
{
	ft_file_closer_single(command);
	ft_free_child(data, tokens, env_copy);
	exit(EXIT_FAILURE);
}

void	dup_input(t_parser *command, t_pipex *data, t_list *tokens, t_list **env_copy)
{
	if (command->exit_code != 0)
		error_closing(command, data, tokens, env_copy);
	if (command->heredoc)
		dup_heredoc(command);
	else if (command->fd_in != -1)
	{
		if (dup2(command->fd_in, 0) == -1)
		{
			perror("dup2: ");
			error_closing(command, data, tokens, env_copy);
		}
		close(command->fd_in);
		command->fd_in = -1;
	}
	else 
	{
		if (data->n2 != 0)
		{
			if (dup2(data->p[(data->n2 * 2) - 2], 0) == -1)
			{
				perror("dup2: ");
				error_closing(command, data, tokens, env_copy);		
			}
		}
	}	
}
void	ft_free_child(t_pipex *data, t_list *tokens, t_list **env_copy)
{
	t_list *tmp;

	tmp = tokens; 
	while (tmp)
	{
		ft_file_closer_single((t_parser *)(tmp->content));
		tmp = tmp -> next;
	}
	ft_free_parser(tokens);
	free (data->pid);
	ft_pipe_closer(data);
	free (data->p);
	ft_lstclear (env_copy, free);
}

void dup_output(t_parser *command, t_pipex *data, t_list *tokens, t_list **env_copy)
{
	if (command->fd_out != -1)
	{
		if (dup2(command->fd_out, 1) == -1)
		{
			perror("dup21 ");
			error_closing(command, data, tokens, env_copy);
		}
		close(command->fd_out);
		command->fd_out = -1;
	}
	else 
	{
		if (data->n2 < data->nbr_p)
		{
			
			if (dup2(data->p[1 + (2 * data->n2)], 1) == -1)
			{
				perror("dup2 ");
				error_closing(command, data, tokens, env_copy);
			}
		}
	}	
}

void	cmd_not_found(t_parser *command, t_list *tokens, t_pipex *data, t_list **env_copy)
{
	char *tmp;
	
	if (command->cmd_args == NULL)
	{
		ft_file_closer_single(command);
		ft_free_child(data, tokens, env_copy);
		exit (EXIT_SUCCESS);
	}
	tmp = ft_strjoin(command->cmd_args[0], ": command not found\n");
	if (tmp == NULL)
	{
		perror("malloc: ");
		ft_free_child(data, tokens, env_copy);
		exit (MALLOC_ERR);
	}
	ft_putstr_fd(tmp, STDERR_FILENO);
	free(tmp);
	ft_file_closer_single(command);
	ft_free_child(data, tokens, env_copy);
	exit (CMD_NOT_FOUND);
}

int n_child_process(t_parser *command, t_list **env_copy, t_pipex *data, t_list *tokens)
{
	data->pid[data->n2] = fork();
	if (data->pid[data->n2] == -1)
	{
		perror("fork: ");
		return (EXIT_FAILURE);
	}
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
			data->exit_code = run_builtins(command, env_copy, data->exit_code, 1);
			ft_free_child(data, tokens, env_copy);
			exit (data->exit_code);
		}
		else 
			execution(command, *env_copy); //HIER MUSS NOCH WAS HIN		
	}
	else 
		ft_file_closer_single(command);
	return (EXIT_SUCCESS);
}

int    create_pipes(t_pipex *data)
{
    int    i;
	int		del;

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

int preperation_for_child(t_pipex *data, t_list *tokens)
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
	int status;

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
	t_pipex data;
	t_list	*tmp;
	//int		n; 
	
	tmp = tokens;
	data.n2 = 0;
	data.exit_code = exit_code;	
	if (preperation_for_child(&data, tokens))
		return (EXIT_FAILURE);
	while (tmp != NULL)
	{
		if (n_child_process((t_parser *)(tmp->content), env_copy, &data, tokens))
			return (EXIT_FAILURE);
		data.n2++;
		tmp = tmp->next;
	}
	return (waiting_for_childs(&data, data.n2));
}




// das war eine alte funktion aber ich wusste nicht mehr in welchen case man direkt von stdin in ein file schreibt 
/* void dup_output(t_parser *command, t_pipex *data, int n)
{
	//char buffer[1];

	if (command->fd_out != -1)
	{
		if (command->cmd_path == NULL)
		{
			ft_pipe_closer(data);
			while (read(STDIN_FILENO, buffer, 1) > 0)
   				write(command->fd_out, buffer, 1);
		}
		else
			dup2(command->fd_out, 1);
		close(command->fd_out);
		command->fd_out = -1;
	}
	else 
	{
		if (n < data->nbr_p)
		{
			if(dup2(data->p[1 + (2 * n)], 1) == -1)
				perror("dupn");
		}
	}	
} */