/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:29:53 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 16:34:56 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_change_to_dir(char *cmd, t_list **env_copy)
{
	char	old_pwd[4097];
	char	*error;

	if (getcwd(old_pwd, 4096) == NULL)
	{
		error = ft_strjoin("cd: ", cmd);
		perror(error);
		free(error);
		return (EXIT_FAILURE);
	}
	if (chdir(cmd) == -1)
	{
		error = ft_strjoin("cd: ", cmd);
		perror(error);
		free(error);
		return (EXIT_FAILURE);
	}
	ft_set_old_env(env_copy, old_pwd);
	ft_set_new_env(env_copy);
	return (EXIT_SUCCESS);
}

//cd function
// - cd => change dir to home
// - cd - => change dir to OLDPWD
// - cd - => with Path! 
int	cd_builtin(char **cmd, t_list **env_copy)
{
	int	i;

	i = 0;
	while (cmd[i])
		i++;
	if (i > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else if (cmd[1] == NULL)
		return (ft_change_to_home(env_copy));
	else if (!ft_strncmp(cmd[1], "-", 2))
		return (ft_change_to_previous(env_copy));
	else if (cmd[1])
		return (ft_change_to_dir(cmd[1], env_copy));
	return (EXIT_SUCCESS);
}
