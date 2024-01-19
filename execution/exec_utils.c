/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwechsle <fwechsle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:21:35 by fwechsle          #+#    #+#             */
/*   Updated: 2024/01/19 17:58:08 by fwechsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	dup_heredoc(t_parser *command)
{
	int	tmp_pipe[2];

	if (pipe(tmp_pipe) == -1)
	{
		perror("pipe: ");
		exit(EXIT_FAILURE);
	}
	write(tmp_pipe[1], command->heredoc, ft_strlen(command->heredoc));
	close(tmp_pipe[1]);
	if (dup2(tmp_pipe[0], 0) == -1)
	{
		perror("dup2: ");
		close(tmp_pipe[0]);
		close(tmp_pipe[1]);
		ft_file_closer_single(command);
		exit(EXIT_FAILURE);
	}
	close(tmp_pipe[0]);
}
