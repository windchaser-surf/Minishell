/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 10:39:48 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 18:24:05 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del(void *content)
{
	free(content);
}

int	print_env(t_list *env_copy)
{
	t_list	*tmp;

	tmp = env_copy;
	while (tmp != NULL)
	{
		if (ft_strchr(tmp->content, '='))
			printf("%s\n", (char *)(tmp->content));
		tmp = tmp -> next;
	}
	return (EXIT_SUCCESS);
}

int	init_env(char **env, t_list **env_copy)
{
	char	*new_content;
	t_list	*new_element;

	if (env == NULL)
		return (EXIT_FAILURE);
	*env_copy = NULL;
	while (*env != NULL)
	{
		new_content = ft_strdup(*env);
		if (new_content == NULL)
			return (ft_lstclear(env_copy, del), EXIT_FAILURE);
		new_element = ft_lstnew((void *)new_content);
		if (new_element == NULL)
		{
			free(new_content);
			return (ft_lstclear(env_copy, del), EXIT_FAILURE);
		}
		ft_lstadd_back(env_copy, new_element);
		env++;
	}
	return (EXIT_SUCCESS);
}
