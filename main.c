/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:43:48 by rluari            #+#    #+#             */
/*   Updated: 2023/12/17 18:46:18 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <readline/readline.h>

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
				printf("%s ", parser_node->cmd_args[i]);
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

int main(int argc, char **argv, char **envp)
{
	char	*command;
	t_list	*lexed_list;
	t_list	**env_copy;
	t_list	*parsed_list;
	int		exit_code;
	
	env_copy = (t_list **)malloc(sizeof(t_list *));
	if (init_env(envp, env_copy) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	//ft_print_env(*env_copy);
	(void)argv;
	if (argc > 1)
		return (ft_putstr_fd("Error: arguments are not accepted.\n", 2), 1);
	exit_code = 0;
	while (1)
	{
		command = readline("Minishell> ");
		add_history(command); 	
		
		if (ft_basic_error_checker(&command) == 1)	// 1 if error, 0 if correct.
			continue;

		lexed_list = ft_lexer(command);	// This function will tokenize the command and store it in a linked list called t_lexer.
		//ft_print_lexer_list(lexed_list);
		//free(command);
		ft_expander(&lexed_list, env_copy);
		//continue;
		parsed_list = ft_parser(lexed_list, env_copy);
		if (!parsed_list)
			continue ;
		/*if (!parsed_list)
			continue ;*/
		//exit_code = ((t_parser *)(ft_lstlast(parsed_list)->content))->exit_code;
		ft_print_parser_list(&parsed_list);
		
		// execution();
		//close the open fd-s
		ft_free_lexer(lexed_list);
		ft_free_parser(parsed_list);
		
	}
	
	//ft_free_env(env_copy);
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