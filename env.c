/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 10:39:48 by fwechsle          #+#    #+#             */
/*   Updated: 2023/12/15 13:13:54 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del(void *content)
{
	free(content);
}

int print_env(t_list *env_copy)
{
	t_list *tmp;
	
	tmp = env_copy;
	while (tmp != NULL)
	{
		if(ft_strchr(tmp->content, '='))
			printf("%s\n", (char *)(tmp->content));
		tmp = tmp -> next;
	}
	return (EXIT_SUCCESS);
}

void	free_env_ll(t_list *env_copy)
{
	t_list *tmp;
	
	tmp = env_copy;
	while (tmp != NULL)
	{
		free(tmp->content);
		tmp = tmp -> next;
	}
}

int init_env(char **env, t_list **env_copy)
{
	char	*new_content;
	t_list	*new_element;
	
	*env_copy = NULL; 
	if (env == NULL)
		return (EXIT_FAILURE); //Exit MINISHELL?
	while (*env != NULL)
	{
		new_content = ft_strdup(*env);
		if (new_content == NULL)
		{
			ft_lstclear(env_copy, del);
			return (EXIT_FAILURE);	//MALLOC ERROR
		}
		new_element = ft_lstnew((void *)new_content);
		if (new_element == NULL)
		{
			ft_lstclear(env_copy, del);
			return (EXIT_FAILURE);
		}
		ft_lstadd_back(env_copy, new_element);
		env++;
	}
	return (EXIT_SUCCESS);
}