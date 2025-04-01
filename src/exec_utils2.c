/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 01:49:08 by jeremias          #+#    #+#             */
/*   Updated: 2025/04/01 05:25:27 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

pid_t	*create_pids_array(int size)
{
	pid_t	*arr;

	if (size <= 0)
		return (NULL);
	arr = (pid_t *)malloc(sizeof(pid_t) * size);
	if (!arr)
		return (NULL);
	ft_memset(arr, 0, sizeof(pid_t) * size);
	return (arr);
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

void	update_shell_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

void wait_for_children(pid_t *pids, int count, t_shell *shell, int prev_pipe)
{
    int i;
    int status;

	(void)prev_pipe;
    i = 0;
    while (i < count)
    {
        if (pids[i] > 0)
        {
            while (waitpid(pids[i], &status, 0) == -1)
            {
                if (errno == EINTR)
                    continue;
                else
                    break;
            }
            if (WIFSIGNALED(status))
            {
                int signum = WTERMSIG(status);
                if (signum == SIGINT)
                    write(STDOUT_FILENO, "\n", 1);
                shell->exit_status = 128 + signum;
            }
            else if (WIFEXITED(status))
                shell->exit_status = WEXITSTATUS(status);
        }
        i++;
    }
}
