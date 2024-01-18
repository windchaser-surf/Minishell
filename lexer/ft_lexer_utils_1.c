/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 09:45:51 by rluari            #+#    #+#             */
/*   Updated: 2024/01/18 21:14:20 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



_Bool	ft_handle_lexer_new_command(t_lexer_helper *h, char *cmd)
{
	if (ft_make_lnode(h, cmd) == 1)
		return (1);
	h->exec_num++;
	h->i++;
	ft_skip_spaces(cmd, &(h->i));
	//if (!(isalnum(cmd[h->i]) || (cmd[h->i] == '>' || cmd[h->i] == '<'))) //for this case: cat asd | | >of1
	if (ft_check_word_first_letter(cmd[h->i], h->list_head) == 1)
	{
		//ft_putstr_fd("Minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	h->start = h->i;
	h->prev_wt = WORD;
	return (0);
}

char	*ft_remove_backslash(char *str)
{
	char	*new_str;
	int		bcks;
	int		i;
	_Bool	was_bck;

	if (ft_strchr(str, '\\') == NULL)
		return (str);
	was_bck = 0;
	i = 0;
	bcks = 0;
	if (str == NULL)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\')
			bcks++;
		i++;
	}
	int asd = - bcks + (bcks/2);
	new_str = (char *)malloc(sizeof(char) * (strlen(str) + asd + 1));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	bcks = 0;	//bcks becomes j which is for str
	while (str[bcks])
	{
		if (str[bcks] != '\\')
			new_str[i++] = str[bcks++];
		else if (*str == '\\' && was_bck == 1)
		{
			new_str[i++] = str[bcks++];
			was_bck = 0;
		}
		else
		{
			was_bck = 1;
			bcks++;
		}
	}
	new_str[i] = '\0';
	free(str);
	return(new_str);
}

_Bool	ft_make_lnode(t_lexer_helper *helper, char *command)
{
	t_lexer	*lexer_node;
	t_list	*new;

	if (helper->start == helper->i)
		return (0);
	lexer_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (lexer_node == NULL)
		return (ft_free_lexer(helper->list_head), perror("Malloc failed"), 1);
	lexer_node->word = ft_substr(command, helper->start, helper->i - helper->start);
	lexer_node->word = ft_remove_backslash(lexer_node->word);
	if (lexer_node->word == NULL)
		return (ft_free_lexer(helper->list_head), free(lexer_node), perror("Malloc failed"), 1);
	lexer_node->type = helper->prev_wt;
	lexer_node->exec_num = helper->exec_num;
	lexer_node->wasnt_empty_var = 1;
	new = ft_lstnew(lexer_node);
	if (new == NULL)
		return (ft_free_lexer(helper->list_head), free(lexer_node->word), free(lexer_node), perror("Malloc failed"), 1);
	ft_lstadd_back(&(helper->list_head), new);
	if (ft_lstsize(helper->list_head) == 1)
		helper->list_head = new;
	return (0);
}

void	ft_init_lexer_helper(t_lexer_helper *helper, char *command, int exec_num)
{
	helper->i = 0;
	helper->exec_num = exec_num;
	helper->prev_wt = WORD;
	helper->list_head = NULL;
	ft_skip_spaces(command + helper->i, &(helper->i));
	helper->start = helper->i;
}

int	ft_check_word_first_letter(char c, t_list *lexer_head)
{
	if (ft_isalnum(c) == 0 && c != ' ' && c != '\"' && c != '\''
		&& c != '$' && c != '>' && c != '<' && c != '/' && c != '.')
	{
		ft_putstr_fd("Minishell: syntax error near unexpected token  `", 2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("'\n", 2);
		ft_free_lexer(lexer_head);
		return (1);
	}
	return (0);	//no error
}
