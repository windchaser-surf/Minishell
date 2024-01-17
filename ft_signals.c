/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 19:56:13 by rluari            #+#    #+#             */
/*   Updated: 2024/01/16 20:37:54 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>


int	g_sig = 0;

void	ft_sighandle_cmd(int sig)
{
	if (sig == SIGINT)
	{
		g_sig = CNTRL_C;
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
		g_sig = CNTRL_C;
		ioctl(STDOUT_FILENO, TIOCSTI, "\n");
		rl_on_new_line();	//swap maybe with rl_replace_line?
		rl_replace_line("", 0);
	}
}

void	ft_init_signals(SigTyp *sig_situation)
{
	if (*sig_situation == INFILE)
	{
		signal(SIGINT, &ft_sighandle_cmd);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (*sig_situation == NOT_INPUT)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (*sig_situation == CHILD)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (*sig_situation == HEREDOC_INP)
	{
		signal(SIGINT, &ft_sighandle_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
}

void	ft_set_mode(SigTyp *sig_mode, SigTyp mode)
{
	*sig_mode = mode;
	ft_init_signals(sig_mode);
}