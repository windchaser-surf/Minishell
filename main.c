/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:43:48 by rluari            #+#    #+#             */
/*   Updated: 2024/01/18 21:19:36 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
//#include <termios.h>

void	ft_print_parser_list(t_list **parser_head)
{
	t_list		*tmp;
	t_parser	*parser_node;
	int			i;

	tmp = *parser_head;
	while (tmp)
	{
		parser_node = (t_parser *)tmp->content;
		i = 0;
		printf("cmd_path: %s\n", parser_node->cmd_path);
		printf("cmd_args: ");
		if (parser_node->cmd_args)
		{	
			while (parser_node->cmd_args[i])
			{
				printf("%s |", parser_node->cmd_args[i]);
				i++;
			}
		}
		printf("\n");
		printf("fd_in: %d\n", parser_node->fd_in);
		printf("fd_out: %d\n", parser_node->fd_out);
		printf("exit_code: %d\n", parser_node->exit_code);
		printf("heredoc: %s\n", parser_node->heredoc);
		printf("\n");
		tmp = tmp->next;
	}
}

void	ft_print_env(t_list *env_copy)
{
	t_list *tmp;
	
	tmp = env_copy;
	while (tmp != NULL)
	{
		if(ft_strchr(tmp->content, '='))
			printf("%s\n", (char *)(tmp->content));
		tmp = tmp -> next;
	}
}

void ft_print_orig_env(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

static void	init_main_helper(t_main_helper *h, char **argv)
{
	h->lexed_list = NULL;
	h->parsed_list = NULL;
	h->command = NULL;
	h->exit_code = 0;
	h->env_copy = NULL;
	h->sig_mode = NOT_INPUT;
	(void)argv;
}

int g_sig = 0;

int main(int argc, char **argv, char **envp)
{
	t_main_helper	h;

	if (argc > 1)
		return (ft_putstr_fd("Minishell: arguments are not accepted.\n", 2), 1);
	init_main_helper(&h, argv);
	if (init_env(envp, &h.env_copy) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (1)
	{
		g_sig = 0;
		ft_init_signals(INPUT);
		if (isatty(fileno(stdin)))
			h.command = readline("Minishell> ");
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			h.command = ft_strtrim(line, "\n");
			free(line);
		}
		//h.command = readline("Minishell> ");
		ft_init_signals(NOT_INPUT);
		if (!h.command)
			break ;	//breaks the loop if ctrl+d is pressed
		add_history(h.command); 
		if (ft_basic_error_checker(&h.command, &h.exit_code) == 1)	// 1 if error, 0 if correct.
			continue ;
		h.lexed_list = ft_lexer(h.command);	// This function will tokenize the command and store it in a linked list called t_lexer.
		h.lexed_list = ft_expander(&h.lexed_list, &h.env_copy, h.exit_code);

		//ft_print_lexer_list(h.lexed_list);
		
		h.parsed_list = ft_parser(h.lexed_list, &h.exit_code, &h.env_copy);

		ft_free_lexer(h.lexed_list);
		free(h.command);
		if (!h.parsed_list)
			continue ;
		
		//ft_print_parser_list(&h.parsed_list);
		
		h.exit_code = execution_main(h.parsed_list, &h.env_copy, h.exit_code);
		ft_free_parser(h.parsed_list);
	}
	clear_history();
	ft_lstclear(&h.env_copy, del);
	return (h.exit_code);
}