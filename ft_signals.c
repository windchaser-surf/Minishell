/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 19:56:13 by rluari            #+#    #+#             */
/*   Updated: 2024/01/23 10:52:25 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>

void	ft_sighandle_cmd(int sig)
{
	if (sig == SIGINT)
	{
		g_ec = 130;
		ft_putstr_fd("\n", STDERR_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ft_sighandle_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		g_ec = 130;
		ioctl(STDOUT_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	ft_init_signals(t_SigTyp sig_situation)
{
	if (sig_situation == INPUT)
	{
		signal(SIGINT, &ft_sighandle_cmd);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (sig_situation == NOT_INPUT)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (sig_situation == CHILD)
	{
		signal(SIGINT, &ft_sighandle_heredoc);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (sig_situation == HEREDOC_INP)
	{
		signal(SIGINT, &ft_sighandle_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
}
