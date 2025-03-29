/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utiils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 01:49:08 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/29 01:50:23 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_pids_array(pid_t **pids, int size)
{
	*pids = malloc(sizeof(pid_t) * size);
	if (!*pids)
	{
		perror("minishell");
		return (0);
	}
	return (1);
}

void	close_previous_pipe(int *pipe_fd)
{
	if (*pipe_fd != -1)
	{
		close(*pipe_fd);
		*pipe_fd = -1;
	}
}

void	close_pipes(int prev_pipe, int current_pipe[2])
{
	close_previous_pipe(&prev_pipe);
	if (current_pipe[0] != -1)
		close(current_pipe[0]);
	if (current_pipe[1] != -1)
		close(current_pipe[1]);
}
