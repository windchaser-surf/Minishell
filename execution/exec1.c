/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 11:17:48 by fwechsle          #+#    #+#             */
/*   Updated: 2023/12/18 10:42:09 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int exec_builtins(t_parser *command, t_list **env_copy)
{
		if (command->fd_in != -1)
			close (command->fd_in);
		if (command->fd_out != -1)
		{
			command->fd_in = dup(STDOUT_FILENO);
			dup2(command->fd_out, 1);
			close(command->fd_out);
		}
		return(run_builtins(command, env_copy));
		
}

int	child_process(t_parser *command, t_pipex *data, t_list **env_copy)
{
	if (command->heredoc)
	{	
				//PROTECTION everything!!!
		data->p = (int *)malloc(sizeof(int) * 2);
		pipe(data->p);				
		write(data->p[1], command->heredoc, ft_strlen(command->heredoc));
		close(data->p[1]);
		dup2(data->p[0], 0);
		close (data->p[0]);
		free(data->p);
	}
	else if (command->fd_in != -1)
	{
		dup2(command->fd_in, 0);
		close(command->fd_in);
	}
	if (command->fd_out != -1)
	{
		dup2(command->fd_out, 1);
		close(command->fd_out);
	}
	execution(command, env_copy);
		 //EXIT CODE!
	return (EXIT_SUCCESS);
}



int    one_execution(t_parser *command, t_list **env_copy) //file_name later from list
{
	t_pipex data;
	int status;
	status = 0;
	if (check_builtin(command->cmd_args[0]))
		return (exec_builtins(command, env_copy));
	else if (command->cmd_path == NULL)
	{
		//print error msg!! 
		return (CMD_NOT_FOUND);
	}
	else 
	{
		data.pid=(int *)malloc(sizeof(int));
		//PROTECTION
		data.pid[0] = fork();
		if (data.pid[0] == -1)
		{
			perror(command->cmd_args[0]);
			return (EXIT_FAILURE);
		}    
		if (data.pid[0] == 0)
			child_process(command, &data, env_copy);
		else
			waitpid(data.pid[0], &status, 0);
		status = WEXITSTATUS(status);
		return (status);
	}
}

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

int n_child_process(t_parser *command, t_list **env_copy, t_pipex *data, int n)
{
	int	tmp_pipe[2];
	data->pid[n] = fork();

	if (data->pid[n] == 0)
	{
		if (command->heredoc)
		{
					//PROTECTION everything!!!
			pipe(tmp_pipe);				
			write(tmp_pipe[1], command->heredoc, ft_strlen(command->heredoc));
			close(tmp_pipe[1]);
			dup2(tmp_pipe[0], 0);
			close (tmp_pipe[0]);
		}
		else if (command->fd_in != -1)
		{
			dup2(command->fd_in, 0);
			close(command->fd_in);
		}
		else 
		{
			if (n != 0)
				dup2(data->p[(n * 2) - 2], 0);

		}
		
		if (command->fd_out != -1)
		{
			dup2(command->fd_out, 1);
			close(command->fd_out);

		}
		else 
		{
			if (n < data->nbr_p)
				dup2(data->p[1 + (2 * n)], 1);
		}
		ft_pipe_closer(data);
		if (!ft_strncmp(command->cmd_path, "builtin", 8))
			exit (run_builtins(command, env_copy));
		else if (command->cmd_path == NULL)
			exit (127);

		else 
			execution(command, env_copy);		

	}
	return (EXIT_FAILURE);
}

void    create_pipes(t_pipex *data)
{
    int    i;

    i = 0;
    while (i < data->nbr_p)
    {
        pipe(data->p + 2 * i);
        i++;
    }
}

int	n_execution(t_list *tokens, t_list **env_copy)
{
	t_pipex data;
	int		n; 
	int status;
	
	status = 0;
	n = 0;
	data.nbr_p = ft_lstsize(tokens) - 1;
	data.p = (int *)malloc(sizeof(int) * (2*data.nbr_p));
	//protection
	create_pipes(&data);
	//protection
	data.pid = (int *)malloc(sizeof(int) * ft_lstsize(tokens));
	while (tokens != NULL)
	{
		n_child_process((t_parser *)(tokens->content), env_copy, &data, n);
		n++;
		tokens = tokens->next;
	}
	data.n = n;
	ft_pipe_closer(&data);
	//waitpid
	n = 0;
	while (n < data.n)
	{
		waitpid(data.pid[n], &status, 0);
		
		n++;
	}
	status = WEXITSTATUS(status);
	//printf("status = %d", status);
	return (status);
}
//wir müssen noch checken ob wir überhaupt in einen child gehen, ansonsten gehen wir raus!! 

int    execution_main(t_list *tokens, t_list **env_copy)
{
	int p_nbr;

	p_nbr = ft_lstsize(tokens);
	if (p_nbr == 1 && ((t_parser *)(tokens->content))->exit_code == 0) //lst_size = 1; 
		return(one_execution((t_parser *)(tokens->content), env_copy)); 
	else if (p_nbr > 1)
	 	return (n_execution(tokens, env_copy));
   /*  else
		n_execution(arg, env_copy); */
	return (EXIT_SUCCESS);
}