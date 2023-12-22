/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felix <felix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 18:23:05 by fwechsle          #+#    #+#             */
/*   Updated: 2023/12/22 16:03:00 by felix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd_builtin(void)
{
	char cwd[4097];
	
	if (getcwd(cwd, 4096) == NULL)
	{
		perror("pwd");
		return (EXIT_FAILURE);
	}
	printf("%s\n", cwd);
	return (EXIT_SUCCESS);
}

void	ft_set_old_env(t_list **env_copy, char *old_pwd)
{
	t_list *tmp;

	tmp = *env_copy;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, "OLDPWD=", 7))
			break; 
		tmp = tmp -> next;
	}
	if (tmp)
	{
		free(tmp->content);
		tmp->content = ft_strjoin("OLDPWD=", old_pwd);
	}
}

void	ft_set_new_env(t_list **env_copy)
{
	t_list *tmp;
	char	new_pwd[4097];

	if (getcwd(new_pwd, 4096) == NULL)
		printf("Error with getting new cwd\n");
	tmp = *env_copy;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, "PWD=", 4))
			break; 
		tmp = tmp -> next;
	}
	if (tmp)
	{	
		free(tmp->content);
		tmp->content = ft_strjoin("PWD=", new_pwd);
	}
}

int	ft_change_to_home(t_list **env_copy)
{
	char old_pwd[4097];

	if (getcwd(old_pwd, 4096) == NULL)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	if (chdir(getenv("HOME")) == -1)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	ft_set_old_env(env_copy, old_pwd);
	ft_set_new_env(env_copy);	
	return (EXIT_SUCCESS);
}

int	ft_change_to_previous(t_list **env_copy)
{
	char old_pwd[4097];

	if (getcwd(old_pwd, 4096) == NULL)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	if(chdir(getenv("OLDPWD")) == -1)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	ft_set_old_env(env_copy, old_pwd);
	ft_set_new_env(env_copy);	
	return (EXIT_SUCCESS);
}

int ft_change_to_dir(char *cmd, t_list **env_copy)
{
	char old_pwd[4097];
	char *error;

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
int    cd_builtin(char **cmd, t_list **env_copy)
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
	else if (cmd == NULL)
		return (ft_change_to_home(env_copy));
	else if (!ft_strncmp(cmd[1], "-", 2))
		return(ft_change_to_previous(env_copy));
	else if (cmd[1])
		return (ft_change_to_dir(cmd[1], env_copy));
	return (EXIT_SUCCESS);	
}

