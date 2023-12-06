/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:43:48 by rluari            #+#    #+#             */
/*   Updated: 2023/12/06 12:39:32 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	char	*command;
	t_list	*lexed_list;
	t_list	*parsed_list;
	
	(void)envp;
	(void)argv;
	
	if (argc > 1)
		return (ft_putstr_fd("Error: arguments are not accepted.\n", 2), 1);
	while (1)
	{
		command = readline("Minishell> ");
		add_history(command); 	
		
		if (ft_basic_error_checker(&command) == 1)	// 1 if error, 0 if correct.
			continue;

		lexed_list = ft_lexer(command);	// This function will tokenize the command and store it in a linked list called t_lexer.
		ft_print_lexer_list(lexed_list);
		//free(command);
		parsed_list = ft_parser(lexed_list);	// This function will parse the command and store it in a linked list called t_parser.
		//free lexed_list
		ft_free_lexer(lexed_list);
		
		
		// Fork a new process
		break ;
	}

	printf("Exiting MyShell...\n");

	return (0);
}