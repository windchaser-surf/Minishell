/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:43:48 by rluari            #+#    #+#             */
/*   Updated: 2024/01/17 11:54:40 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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

/*void generic_sig_handler(int sig)
{
	if (g_running_process.pid && g_running_process.pid[g_running_process.n] > 0) // TODO: don't use a global variable? (would need to establish signal disposition in run_command())
    {
        kill(g_running_process.pid[g_running_process.n], sig);
        return ;
    }

	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		ft_putstr_fd("\b\b  \b\b", 1);
	}
	
}*/

/*void init_sig(void)
{
	struct sigaction	sa;
	struct termios		current;
	int					tty_fd;

	sa.sa_handler = generic_sig_handler;
	//sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	tcgetattr(0, &current);
	tty_fd = open(ttyname(0), O_RDONLY);
	current.c_lflag |= ISIG;
	tcsetattr(tty_fd, TCSANOW, &current);
	sigaction(SIGQUIT, &sa, NULL);
	close(tty_fd);
}*/

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
		ft_set_mode(&h.sig_mode, INPUT);
		h.command = readline("Minishell> ");
		ft_set_mode(&h.sig_mode, NOT_INPUT);
		if (!h.command)
			continue ;
		add_history(h.command); 
		if (ft_basic_error_checker(&h.command, &h.exit_code) == 1)	// 1 if error, 0 if correct.
			continue;
		h.lexed_list = ft_lexer(h.command);	// This function will tokenize the command and store it in a linked list called t_lexer.
		h.lexed_list = ft_expander(&h.lexed_list, &h.env_copy, h.exit_code);
		ft_print_lexer_list(h.lexed_list);
		h.parsed_list = ft_parser(h.lexed_list, &h.exit_code, &h.env_copy, &h.sig_mode);
		
		ft_free_lexer(h.lexed_list);

		if (!h.parsed_list)
			continue ;
		ft_print_parser_list(&h.parsed_list);
		h.exit_code = execution_main(h.parsed_list, &h.env_copy, h.exit_code);
		
		ft_free_parser(h.parsed_list);
	}
}