/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:25 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/28 19:26:00 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	create_pids_array(pid_t **pids, int size)
{
	*pids = malloc(sizeof(pid_t) * size);
	if (!*pids)
	{
		perror("Failed to allocate memory for pids");
		return (0);
	}
	return (1);
}

static int	setup_child_process(t_cmd_node *cmd, int prev_pipe_in,
	int pipefd[2])
{
	setup_child_signals();
	if (prev_pipe_in != -1)
	{
		dup2(prev_pipe_in, STDIN_FILENO);
		close(prev_pipe_in);
	}
	if (cmd->next)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		dup2(cmd->out_fd, STDOUT_FILENO);
		close(cmd->out_fd);
	}
	if (cmd->in_fd != STDIN_FILENO)
	{
		dup2(cmd->in_fd, STDIN_FILENO);
		close(cmd->in_fd);
	}
	close(pipefd[0]);
	return (1);
}

static int	fork_and_execute(t_cmd_node *cmd, int prev_pipe_in, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		setup_child_process(cmd, prev_pipe_in, pipefd);
		if (!cmd->args || !cmd->args[0])
		{
			fprintf(stderr, "minishell: invalid command\n");
			exit(1);
		}
		execvp(cmd->args[0], cmd->args);
		perror("execvp");
		exit(1);
	}
	return (pid);
}

static int	process_pipeline_loop(t_cmd_node *cmd, pid_t *pids,
	int *prev_pipe_in)
{
	int	i;
	int	pipefd[2];

	i = 0;
	while (cmd)
	{
		if (cmd->args && cmd->args[0])
		{
			if (cmd->next && pipe(pipefd) == -1)
				return (perror("pipe"), -1);
			pids[i] = fork_and_execute(cmd, *prev_pipe_in, pipefd);
			if (pids[i] == -1)
				return (-1);
			i++;
			if (cmd->next)
			{
				close(pipefd[1]);
				*prev_pipe_in = pipefd[0];
			}
			else
				*prev_pipe_in = -1;
		}
		cmd = cmd->next;
	}
	return (i);
}

void	execute_pipeline(t_cmd_list *cmd_list, t_shell *shell)
{
	pid_t		*pids;
	int			prev_pipe_in;
	int			i;
	t_cmd_node	*cmd;

	prev_pipe_in = -1;
	i = 0;
	cmd = cmd_list->head;
	if (!cmd || !create_pids_array(&pids, cmd_list->size))
		return ;
	i = process_pipeline_loop(cmd, pids, &prev_pipe_in);
	if (i == -1)
	{
		free(pids);
		return ;
	}
	wait_for_children(i, pids, shell);
	free(pids);
}
