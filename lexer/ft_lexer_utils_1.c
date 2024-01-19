/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 09:45:51 by rluari            #+#    #+#             */
/*   Updated: 2024/01/19 13:23:35 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



_Bool	ft_handle_lexer_new_command(t_lexer_helper *h, char *cmd, int *exit_code)
{
	if (ft_make_lnode(h, cmd) == 1)
		return (1);
	h->exec_num++;
	h->i++;
	ft_skip_spaces(cmd, &(h->i));
	//if (!(isalnum(cmd[h->i]) || (cmd[h->i] == '>' || cmd[h->i] == '<'))) //for this case: cat asd | | >of1
	if (ft_check_word_first_letter(cmd[h->i], h->list_head, exit_code) == 1)
	{
		//ft_putstr_fd("Minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	h->start = h->i;
	h->prev_wt = WORD;
	return (0);
}

/*char	*ft_remove_backslash(char *str, int *i)
{
	char	*new_str;
	int		bcks;
	_Bool	was_bck;

	was_bck = 0;
	int tmp_i = *i;
	bcks = 0;
	if (str[tmp_i + 1] == '$')
	{
		//skip i to the end of the var name
		//just remove one backslash
	}
	else if (str[tmp_i + 1] == '\\')
	{
		//just remove one backslash
		new_str = (char *)malloc(sizeof(char) * (strlen(str) -1 + 1));
	}
	return(new_str);
}*/

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
	//lexer_node->word = ft_remove_backslash(lexer_node->word);
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

int	ft_check_word_first_letter(char c, t_list *lexer_head, int *exit_code)
{
	if (ft_isalnum(c) == 0 && c != ' ' && c != '\"' && c != '\''
		&& c != '$' && c != '>' && c != '<' && c != '/' && c != '.')
	{
		ft_putstr_fd("Minishell: syntax error near unexpected token  `", 2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("'\n", 2);
		ft_free_lexer(lexer_head);
		if (exit_code)
			*exit_code = 2;
		return (1);
	}
	return (0);	//no error
}
