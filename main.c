/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:43:48 by rluari            #+#    #+#             */
/*   Updated: 2023/12/13 10:48:41 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	char	*command;
	t_list	*lexed_list;
	t_list	*parsed_list;
	int		exit_code;
	
	(void)envp;
	(void)argv;
	
	if (argc > 1)
		return (ft_putstr_fd("Error: arguments are not accepted.\n", 2), 1);
	exit_code = 0;
	//setting the env
	while (1)
	{
		command = readline("Minishell> ");
		add_history(command); 	
		
		if (ft_basic_error_checker(&command) == 1)	// 1 if error, 0 if correct.
			continue;

		lexed_list = ft_lexer(command);	// This function will tokenize the command and store it in a linked list called t_lexer.
		ft_print_lexer_list(lexed_list);
		//free(command);
		ft_print_lexer_list(lexed_list);
		
		parsed_list = ft_parser(lexed_list);	// This function will parse the command and store it in a linked list called t_parser.
		
		ft_free_lexer(lexed_list);
		if (!parsed_list)
			continue ;
		exit_code = ((t_parser *)(ft_lstlast(parsed_list)->content))->exit_code;
		(void)parsed_list;
		
		// execution();
		
		ft_free_parser(parsed_list);
		
	}

	printf("Exiting MyShell...\n");

	return (0);
}

/*
Richi:
1. exit codes, first check for non existent file, set exit code to 1, if it exists, but the command is invalid, set exit code to 127
Felix gives me the copied ENV, 
2. handle $ (variables)
3. '$test' you will not replace it
	"$test" || $test you will replace it

execve("user/bin/grep", "grep, $test")
*/