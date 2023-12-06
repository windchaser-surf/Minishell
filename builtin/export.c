/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:17:52 by felix             #+#    #+#             */
/*   Updated: 2023/12/05 18:31:46 by fwechsle         ###   ########.fr       */
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

void	print_alpha(char **arr)
{
	char			*variable_name;
	char			*variable_value;
	unsigned int	position;
	if (!arr)
		return ; 
	while (*arr)
	{
		if (ft_strchr(*arr, '='))
		{
			variable_value =  ft_strchr(*arr, '=');
			position = variable_value - *arr;
			variable_value++;
			variable_name = ft_substr(*arr, 0, position);
			ft_printf("declare -x %s=\"%s\"\n", variable_name, variable_value);
			free(variable_name);
		}
		else
			printf("declare -x %s\n", *arr);
		arr++;
	}	
}
void	print_export(t_list *env_copy)
{
	int lstsize;
	char **arr;
	int i;

	lstsize = ft_lstsize(env_copy);
	printf("lstsize = %d\n", lstsize);
	arr = (char **)malloc(sizeof(char *) * (lstsize));
	if (arr == NULL)
		return ;    //Error code
	i = 0;
	while (env_copy)
	{
		if ((env_copy->content[0] >= 'A' && env_copy->content[0] <= 'Z')\
			|| (env_copy->content[0] >= 'a' && env_copy->content[0] <= 'z'))
		{
			if (env_copy->content)
				arr[i] = ft_strdup(env_copy->content);
			if (arr[i] == NULL)
				return ; // Error code
			i++;
		}
		env_copy = env_copy->next; 
	}
	arr[i] = NULL;
	arr = sort_arr(arr, lstsize);
	print_alpha(arr);
	free_2d(arr);
}

int	valid_new_variable(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] && cmd[i] != '=')
	{
		if (cmd[i] >= 'A' && cmd[i] <= 'Z')
			i++;
		else if (cmd[i] >= 'a' && cmd[i] <= 'z')
			i++;
		else if (cmd[i] >= '0' && cmd[i] <= '9')
			i++;
		else if (cmd[i] == '_')
			i++;
		else
		{
			printf("export: `%s': not a valid identifier\n", cmd); //Error code
			return (0);
		}
	}
	return (1);
}

int	check_exist(char *var, t_list *env_copy)
{	
	int	i;
	
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	while (env_copy)
	{
		if (!ft_strncmp(env_copy->content, var, i))
			return (1);
		env_copy = env_copy->next;
	}
	return (0);
}

void	set_value_env(char *cmd, t_list *env_copy)
{
	int	i;
	
	i = 0;
	while (cmd[i] && cmd[i] != '=')
		i++;
	if(cmd[i] != '=')
		return ;
	while (env_copy)
	{
		if (!ft_strncmp(env_copy->content, cmd, i))
		{
			free(env_copy->content);
			env_copy->content = ft_strdup(cmd);
		}
		env_copy = env_copy->next;
	}
}
//Before adding a new list element, needs to check if there is already this var! 
//if it exist => change the value! 
void	export_builtin(char *cmd, t_list **env_copy)
{
	if (cmd == NULL)
		print_export(*env_copy);
	else if (valid_new_variable(cmd))
	{
		if(!check_exist(cmd, *env_copy))
			ft_lstadd_back(&*env_copy, ft_lstnew((char *)ft_strdup(cmd)));
		else 
			set_value_env(cmd, *env_copy);
		print_export(*env_copy);
	}
}

//next Step => split cmd beim = dann checke ob die Variable schon existiert
//entscheide dann ob du den Wert überschreibst oder die Variable mit dem Wert anlegst


//unset removes existing but if there is no existing there is no error
