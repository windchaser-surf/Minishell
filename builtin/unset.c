/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 10:50:44 by fwechsle          #+#    #+#             */
/*   Updated: 2023/12/05 16:58:04 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//ich muss hier noch eine Funktion schreiben die ein Element aus der Liste löscht und den rest und den vorherigen Pointer auf den nächsten setzt

void	del_env_var(t_list **env_copy, char *var)
{
    //t_list	**tmp;
	t_list	*current;
    t_list	*prev;

	//tmp = env_copy;
	current = *env_copy;
	prev = NULL;
    // Durchsuche die Liste, um das Element zu finden
    while (current != NULL && ft_strncmp(current->content, var, ft_strlen(var)))
	{
        prev = current;
        current = current->next;
	}

    // Überprüfe, ob das Element am Anfang der Liste steht
    if (prev == NULL) 
    	*env_copy = current->next;
    else 
    {
		prev->next = current->next;
	}
    // Freigabe des Speichers des gelöschten Knotens
    ft_lstdelone(current);
}
void    builtin_unset(t_list **env_copy, char *var)
{
	t_list *tmp;
	
	tmp = *env_copy;
	if (var == NULL)
		return ; //if there is no var nothing happen
	if (check_exist(var, *env_copy) && ft_strchr(var, '=') == NULL)
		{
			while (tmp)
			{
				if (!ft_strncmp((tmp)->content, var, ft_strlen(var))\
					&& ((tmp)->content[ft_strlen(var)] == '=' ||\
					(tmp)->content[ft_strlen(var)] == '\0'))
				{
					del_env_var(env_copy, var);
					return ;
				}	//ft_lstdelone(env_copy);
					//Wir müssen hier noch die Pointer richtig setzen!! die lstdelone ist noch falsch
				(tmp) = (tmp) -> next;
			}
		}
}