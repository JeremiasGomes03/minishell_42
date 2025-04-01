/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamachad <lamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:25 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/31 23:55:26 by lamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	setup_child_process(t_cmd_node *cmd,
	int prev_pipe_in, int pipefd[2])
{
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
	return (1);
}

static int	fork_and_execute(t_cmd_node *cmd, int prev_pipe_in,
	int pipefd[2], t_shell *shell)
{
	pid_t	pid;
	char	*path;
	int		status_pipe[2];
	int		exit_status;

	if (is_env_modifying_builtin(cmd))
		return (handle_parent_builtin(cmd, shell));
	if (pipe(status_pipe) == -1)
		return (perror("minishell: pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), -1);
	if (pid == 0)
	{
		close(status_pipe[0]);
		setup_child_process(cmd, prev_pipe_in, pipefd);
		if (is_builtin(cmd))
		{
			exit_status = execute_builtin(cmd, shell);
			write(status_pipe[1], &exit_status, sizeof(int));
			close(status_pipe[1]);
			exit(exit_status);
		}
		path = get_absolute_path(cmd->args[0], shell);
		if (!path)
			exit(print_error(cmd->args[0], "command not found", 127));
		if (execve(path, cmd->args, shell->envp) == -1)
			exit(print_error(cmd->args[0], strerror(errno), 126));
		free(path);
		exit(shell->exit_status);
	}
	close(status_pipe[1]);
	read(status_pipe[0], &shell->exit_status, sizeof(int));
	close(status_pipe[0]);
	return (pid);
}

static int	handle_command(t_cmd_node **cmd, pid_t *pid,
	int *prev_pipe, t_shell *sh)
{
	int	curr_pipe[2];

	curr_pipe[0] = -1;
	curr_pipe[1] = -1;
	if ((*cmd)->next && pipe(curr_pipe) == -1)
		return (close_pipes(*prev_pipe, curr_pipe), 0);
	if (pid)
		*pid = fork_and_execute(*cmd, *prev_pipe, curr_pipe, sh);
	else
		fork_and_execute(*cmd, *prev_pipe, curr_pipe, sh);
	if (*pid == -1)
		return (close_pipes(*prev_pipe, curr_pipe), 0);
	close_previous_pipe(prev_pipe);
	if ((*cmd)->next)
	{
		*prev_pipe = curr_pipe[0];
		close(curr_pipe[1]);
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
		if (!handle_command(&cmd, &pids[i], prev_p, sh))
			return (-1);
		i++;
	}
	return (i);
}

void	execute_pipeline(t_cmd_list *cmd_list, t_shell *shell)
{
	pid_t	*pids;
	int		prev_pipe;
	int		cmd_count;

	pids = NULL;
	prev_pipe = -1;
	if (!cmd_list || !cmd_list->head || cmd_list->size <= 0)
		return ;
	cmd_count = cmd_list->size;
	pids = create_pids_array(cmd_count);
	if (!pids)
	{
		perror("minishell: malloc failed");
		exit(EXIT_FAILURE);
	}
	ft_memset(pids, 0, sizeof(pid_t) * cmd_count);
	if (process_commands(cmd_list->head, pids, &prev_pipe, shell) == -1)
	{
		free(pids);
		return ;
	}
	if (prev_pipe != -1)
		close(prev_pipe);
	wait_for_children(pids, cmd_count, shell);
	free(pids);
}
