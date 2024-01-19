/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:29:20 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 17:41:42 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_new_variable(char *cmd)
{
	int	i;

	i = 0;
	if (cmd[0] == '=')
		return (err_msg_not_valid());
	while (cmd[i] && cmd[i] != '=')
	{
		if ((cmd[0] >= '0' && cmd[0] <= '9') || cmd[0] == '=')
			return (err_msg_not_valid());
		if (cmd[i] >= 'A' && cmd[i] <= 'Z')
			i++;
		else if (cmd[i] >= 'a' && cmd[i] <= 'z')
			i++;
		else if (cmd[i] >= '0' && cmd[i] <= '9')
			i++;
		else if (cmd[i] == '_')
			i++;
		else
			return (err_msg_not_valid());
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
	if (cmd[i] != '=')
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

int	export_builtin(char **cmd, t_list **env_copy)
{
	int	i;
	int	check2;

	check2 = 0;
	i = 1;
	if (cmd[i] == NULL)
		return (print_export(*env_copy));
	while (cmd[i])
	{
		if (!valid_new_variable(cmd[i]))
		{
			if (!check_exist(cmd[i], *env_copy))
				ft_lstadd_back(&*env_copy, \
				ft_lstnew((char *)ft_strdup(cmd[i])));
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
