/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 18:23:05 by fwechsle          #+#    #+#             */
/*   Updated: 2023/12/05 18:24:02 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd_builtin(void)
{
	char cwd[4097];
	
	if (getcwd(cwd, 4096) == NULL)
		printf("Error with PWD\n"); //Error message;
	printf("%s\n", cwd);
}

void	set_old_env(t_list **env_copy, char *old_pwd)
{
	t_list *tmp;

	tmp = *env_copy;
	while (tmp->content)
	{
		if (!ft_strncmp(tmp->content, "OLDPWD=", 7))
			break; 
		tmp = tmp -> next;
	}
	free(tmp->content);
	tmp->content = ft_strjoin("OLDPWD=", old_pwd);
}

void	set_new_env(t_list **env_copy)
{
	t_list *tmp;
	char	new_pwd[4097];

	if (getcwd(new_pwd, 4096) == NULL)
		printf("Error with getting new cwd\n");
	tmp = *env_copy;
	while (tmp->content)
	{
		if (!ft_strncmp(tmp->content, "PWD=", 4))
			break; 
		tmp = tmp -> next;
	}
	free(tmp->content);
	tmp->content = ft_strjoin("PWD=", new_pwd);
}
//cd function
// - cd => change dir to home
// - cd - => change dir to OLDPWD
// - cd - => with Path! 
void    cd_builtin(char *cmd, t_list **env_copy)
{
	char old_pwd[4097];

	if (cmd == NULL)
	{
		if (getcwd(old_pwd, 4096) == NULL)
			printf("Error with getting cwd\n");
		if (chdir(getenv("HOME")) == -1)
			return ; //print error code
	}
	else if (!ft_strncmp(cmd, "-", 2))
	{
		if (getcwd(old_pwd, 4096) == NULL)
			printf("Error with getting cwd\n");
		if(chdir(getenv("OLDPWD")) == -1)
			printf("Error with changing to OLDPWD");
	}
	else if (cmd)
	{
		if (getcwd(old_pwd, 4096) == NULL)
			printf("Error with getting cwd\n");
		if (chdir(cmd) == -1)
			printf("cd: no such file or directory: %s\n", cmd);
	}
	set_old_env(env_copy, old_pwd);
	set_new_env(env_copy);		
}