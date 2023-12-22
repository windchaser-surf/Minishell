/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felix <felix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:17:52 by felix             #+#    #+#             */
/*   Updated: 2023/12/22 16:09:57 by felix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**sort_arr(char **arr, int size)
{
	int		io;
	int		ii;
	int		loop;
	char	*temp;

	ii = 0;
	io = 0;
	loop = size - 1;
	while (io < loop)
	{
		while (ii < loop)
		{
			if (ft_strncmp(arr[ii], arr[ii + 1], ft_strlen(arr[ii])) > 0)
			{
				temp = arr[ii];
				arr[ii] = arr[ii + 1];
				arr[ii + 1] = temp;
			}
			ii++;
		}
		ii = 0;
		io++;
	}
	return (arr);
}

int	print_alpha(char **arr)
{
	char			*variable_name;
	char			*variable_value;
	unsigned int	position;
	if (!arr)
		return (EXIT_FAILURE); 
	while (*arr)
	{
		if (ft_strchr(*arr, '='))
		{
			variable_value =  ft_strchr(*arr, '=');
			position = variable_value - *arr;
			variable_value++;
			variable_name = ft_substr(*arr, 0, position);
			if (variable_name == NULL)
			{
				perror("export");
				return (MALLOC_ERR);
			}
			printf("declare -x %s=\"%s\"\n", variable_name, variable_value);
			free(variable_name);
		}
		else
			printf("declare -x %s\n", *arr);
		arr++;
	}
	return (EXIT_SUCCESS);
}
int	print_export(t_list *env_copy)
{
	int lstsize;
	char **arr;
	int i;

	lstsize = ft_lstsize(env_copy);
	arr = (char **)malloc(sizeof(char *) * (lstsize));
	if (arr == NULL)
	{
		perror("export");
		return (MALLOC_ERR);
	}	  //Error code
	i = 0;
	while (env_copy)
	{
		if ((((char *)env_copy->content)[0] >= 'A' && ((char *)env_copy->content)[0] <= 'Z')\
			|| (((char *)env_copy->content)[0] >= 'a' && ((char *)env_copy->content)[0] <= 'z'))
		{
			if (env_copy->content)
				arr[i] = ft_strdup(env_copy->content);
			if (arr[i] == NULL)
			{
				perror("export");
				return (MALLOC_ERR);
			}	
			i++;
		}
		env_copy = env_copy->next; 
	}
	arr[i] = NULL;
	arr = sort_arr(arr, lstsize);
	if (print_alpha(arr) == EXIT_FAILURE)
	{
		free_2d(arr);
		return (EXIT_FAILURE);
	}
	free_2d(arr);
	return (EXIT_SUCCESS);
}

int	err_msg_not_valid(void)
{
	ft_putstr_fd("export: Not a valid identifier\n", STDERR_FILENO);
	return (EXIT_FAILURE);	
}

int	valid_new_variable(char *cmd)
{
	int	i;

	i = 0;
	if (cmd[0] == '=')
		return (err_msg_not_valid());
	while (cmd[i] && cmd[i] != '=')
	{
		if ((cmd[0] >= '0' && cmd[0] <= '9') || cmd[0] == '=')
			return(err_msg_not_valid());
		if (cmd[i] >= 'A' && cmd[i] <= 'Z')
			i++;
		else if (cmd[i] >= 'a' && cmd[i] <= 'z')
			i++;
		else if (cmd[i] >= '0' && cmd[i] <= '9')
			i++;
		else if (cmd[i] == '_')
			i++;
		else
			return(err_msg_not_valid());
	}
	return (EXIT_SUCCESS);
}

int	check_exist(char *var, t_list *env_copy)
{	
	int	i;
	
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	while (env_copy)
	{
		if (!ft_strncmp(env_copy->content, var, i + 1))
			return (1);
		env_copy = env_copy->next;
	}
	return (0);
}

int	set_value_env(char *cmd, t_list *env_copy)
{
	int	i;
	
	i = 0;
	while (cmd[i] && cmd[i] != '=')
		i++;
	if(cmd[i] != '=')
		return (EXIT_SUCCESS);
	while (env_copy)
	{
		if (!ft_strncmp(env_copy->content, cmd, i + 1))
		{
			free(env_copy->content);
			env_copy->content = ft_strdup(cmd);
			if (!env_copy->content)
			{
				perror("export");
				return (EXIT_FAILURE);
			}
		}
		env_copy = env_copy->next;
	}
	return (EXIT_SUCCESS);
}
//Before adding a new list element, needs to check if there is already this var! 
//if it exist => change the value! 
int	export_builtin(char **cmd, t_list **env_copy)
{
	int i;
	int check2; 

	check2 = 0;
	i = 1; 
	if (cmd[i] == NULL)
		return(print_export(*env_copy));
	while (cmd[i])
	{
		if (!valid_new_variable(cmd[i]))
		{
			if(!check_exist(cmd[i], *env_copy))
				ft_lstadd_back(&*env_copy, ft_lstnew((char *)ft_strdup(cmd[i]))); //malloc protection? 
			else 
				set_value_env(cmd[i], *env_copy);
		}
		else
			check2 = 1;
		i++;
	}	
	if (check2 == 1)
		return (EXIT_FAILURE);
	else 
		return (EXIT_SUCCESS);
}

//Hier muss noch mehrer auf einmal hinzufügt werden. das bedeutet dann einfach ich werden einen while loop machen
//muss aber noch *cmd mit **arg ersetzen und dann durch loopen 
//Sobald eines der namen falsch ist muss error_code 1 returned werden aber es werden trotzdem alle validen ausgeführt

