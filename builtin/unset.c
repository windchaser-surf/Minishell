/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 10:50:44 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 17:51:07 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	del_env_var(t_list **env_copy, char *var)
{
	t_list	*current;
	t_list	*prev;

	current = *env_copy;
	prev = NULL;
	while (current != NULL && \
		(ft_strncmp(current->content, var, ft_strlen(var)) \
		|| (((char *)(current)->content)[ft_strlen(var)] != '=')) && \
		((char *)(current->content))[ft_strlen(var) != 0])
	{
		prev = current;
		current = current->next;
	}
	if (prev == NULL)
		*env_copy = current->next;
	else
	{
		if (current)
			prev->next = current->next;
	}
	if (current)
		ft_lstdelone(current, del);
}

int	check_exist_unset(char *var, t_list *env_copy)
{
	int		i;
	t_list	*tmp;

	tmp = env_copy;
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, var, i))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	builtin_unset(t_list **env_copy, char **var, int i)
{
	t_list	*tmp;

	if (var[i] == NULL)
		return (EXIT_SUCCESS);
	while (var[i])
	{
		tmp = *env_copy;
		if (check_exist_unset(var[i], tmp) && ft_strchr(var[i], '=') == NULL)
		{
			while (tmp)
			{
				if (!ft_strncmp(((char *)(tmp)->content), var[i], \
					ft_strlen(var[i])) \
					&& (((char *)(tmp)->content)[ft_strlen(var[i])] == '=' || \
					((char *)(tmp)->content)[ft_strlen(var[i])] == '\0'))
				{
					del_env_var(env_copy, var[i]);
					break ;
				}
				tmp = tmp -> next;
			}
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
