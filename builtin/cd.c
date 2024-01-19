/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 18:23:05 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 16:34:20 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd_builtin(void)
{
	char	cwd[4097];

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
	t_list	*tmp;

	tmp = *env_copy;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, "OLDPWD=", 7))
			break ;
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
	t_list	*tmp;
	char	new_pwd[4097];

	if (getcwd(new_pwd, 4096) == NULL)
		printf("Error with getting new cwd\n");
	tmp = *env_copy;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, "PWD=", 4))
			break ;
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
	char	old_pwd[4097];

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
	char	old_pwd[4097];

	if (getcwd(old_pwd, 4096) == NULL)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	if (chdir(getenv("OLDPWD")) == -1)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	ft_set_old_env(env_copy, old_pwd);
	ft_set_new_env(env_copy);
	return (EXIT_SUCCESS);
}
