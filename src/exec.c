/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:25 by jeremias          #+#    #+#             */
/*   Updated: 2025/04/01 05:19:48 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int setup_child_process(t_cmd_node *cmd, int prev_pipe_in, int pipefd[2])
{
	if (prev_pipe_in != -1)
	{
		dup2(prev_pipe_in, STDIN_FILENO);
		close(prev_pipe_in);
	}
	if (cmd->next)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (cmd->in_fd != STDIN_FILENO)
	{
		dup2(cmd->in_fd, STDIN_FILENO);
		close(cmd->in_fd);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		dup2(cmd->out_fd, STDOUT_FILENO);
		close(cmd->out_fd);
	}
	return (1);
}

static int	fork_and_execute(t_cmd_node *cmd, int prev_pipe_in,
	int pipefd[2], t_shell *shell)
{
	pid_t	pid;
	char	*path;
	int		exit_status;

	if (is_env_modifying_builtin(cmd))
		return (handle_parent_builtin(cmd, shell));
	if (is_builtin(cmd))
	{
		int status_pipe[2];

		if (pipe(status_pipe) == -1)
			return (perror("minishell: pipe"), -1);
		pid = fork();
		if (pid == -1)
			return (perror("minishell: fork"), -1);
		if (pid == 0)
		{ 
			close(status_pipe[0]);
			setup_child_process(cmd, prev_pipe_in, pipefd);
			exit_status = execute_builtin(cmd, shell);
			write(status_pipe[1], &exit_status, sizeof(int));
			close(status_pipe[1]);
			exit(exit_status);
		}
		close(status_pipe[1]);
		read(status_pipe[0], &shell->exit_status, sizeof(int));
		close(status_pipe[0]);
		return (pid);
	}
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), -1);
	if (pid == 0)
	{
		setup_child_process(cmd, prev_pipe_in, pipefd);
		path = get_absolute_path(cmd->args[0], shell);
		if (!path)
			exit(print_error(cmd->args[0], "command not found", 127));
		if (execve(path, cmd->args, shell->envp) == -1)
			exit(print_error(cmd->args[0], strerror(errno), 126));
		free(path);
		exit(shell->exit_status);
	}
	return (pid);
}

static int handle_command(t_cmd_node **cmd, pid_t *pid,
	int *prev_pipe, t_shell *sh)
{
	int	curr_pipe[2];

	curr_pipe[0] = -1;
	curr_pipe[1] = -1;
	if ((*cmd)->next && pipe(curr_pipe) == -1)
	{
		close_pipes(*prev_pipe, curr_pipe);
		return (0);
	}
	*pid = fork_and_execute(*cmd, *prev_pipe, curr_pipe, sh);
	if (*prev_pipe != -1)
		close(*prev_pipe);
	if ((*cmd)->next)
	{
		close(curr_pipe[1]);
		*prev_pipe = curr_pipe[0];
	}
	else
		close(curr_pipe[0]);
	*cmd = (*cmd)->next;
	return (1);
}


static int	process_commands(t_cmd_node *cmd, pid_t *pids,
	int *prev_p, t_shell *sh)
{
	int	i;

	i = 0;
	while (cmd)
	{
		if (cmd->in_fd == -2 || cmd->out_fd == -2)
		{
			sh->exit_status = 130;
			close_pipes(*prev_p, (int [2]){-1, -1});
			return (0);
		}
		if (!handle_command(&cmd, &pids[i], prev_p, sh))
			return (-1);
		i++;
	}
	return (i);
}

void execute_pipeline(t_cmd_list *cmd_list, t_shell *shell)
{
    pid_t *pids;
    int prev_pipe;
    int cmd_count;

    prev_pipe = -1;
    cmd_count = cmd_list->size;
    pids = create_pids_array(cmd_count);
    if (!pids)
        return (perror("minishell: malloc failed"), (void)0);
    process_commands(cmd_list->head, pids, &prev_pipe, shell);
    wait_for_children(pids, cmd_count, shell, prev_pipe);
    free(pids);
}
