/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:43:48 by rluari            #+#    #+#             */
/*   Updated: 2024/01/03 19:10:00 by rluari           ###   ########.fr       */
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

int main(int argc, char **argv, char **envp)
{
	char	*command;
	t_list	*lexed_list;
	t_list	*env_copy;
	t_list	*parsed_list;
	int		exit_code;
	
	if (argc > 1)
		return (ft_putstr_fd("Minishell: arguments are not accepted.\n", 2), 1);
	if (init_env(envp, &env_copy) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	(void)argv;
	//ft_print_env(env_copy);
	exit_code = 0;
	while (1)
	{
		command = readline("Minishell> ");
		if (ft_basic_error_checker(&command, &exit_code) == 1)	// 1 if error, 0 if correct.
			continue;
		add_history(command); 
		
		lexed_list = ft_lexer(command);	// This function will tokenize the command and store it in a linked list called t_lexer.
		//free(command);
		ft_expander(&lexed_list, &env_copy, exit_code);
		//ft_print_lexer_list(lexed_list);
		parsed_list = ft_parser(lexed_list, &exit_code, &env_copy);
		ft_free_lexer(lexed_list);
		if (!parsed_list)
			continue ;
		/*if (!parsed_list)
			continue ;*/
		//exit_code = ((t_parser *)(ft_lstlast(parsed_list)->content))->exit_code;
		//ft_print_parser_list(&parsed_list);
		exit_code = execution_main(parsed_list, &env_copy, exit_code);
		ft_free_parser(parsed_list);
	}
}