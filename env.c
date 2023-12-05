/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 10:39:48 by fwechsle          #+#    #+#             */
/*   Updated: 2023/12/04 16:51:25 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_env(t_list *env_copy)
{
	t_list *tmp;
	
	tmp = env_copy;
	while (tmp != NULL)
	{
		if(ft_strchr(tmp->content, '='))
			printf("%s\n", tmp->content);
		tmp = tmp -> next;
	}
}

void init_env(char **env, t_list **env_copy)
{
	*env_copy = NULL; 
	if (env == NULL)
		return ; //Exit MINISHELL?
	while (*env != NULL)
	{
		ft_lstadd_back(&*env_copy, ft_lstnew((void *)ft_strdup(*env)));
		env++;
	}
	//print_all(*env_copy);
}