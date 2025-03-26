/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:25 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/26 17:16:27 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_command_args(t_cmd_node *cmd, t_shell *shell)
{
	int		i;
	char	*expanded_arg;
	char	*temp;
	int		quotes;

	i = 0;
	quotes = 0;
	while (cmd->args && cmd->args[i])
	{
		quotes = check_quotes(cmd->args[i][0], quotes);
		if (cmd->args[i][0] == '\1')
		{
			temp = ft_strdup(cmd->args[i] + 1);
			if (!temp)
			{
				perror("ft_strdup");
				exit(EXIT_FAILURE);
			}
			free(cmd->args[i]);
			cmd->args[i] = temp;
		}
		else if (ft_strchr(cmd->args[i], '$') && quotes != 2)
		{
			expanded_arg = expand_variables(cmd->args[i], shell);
			if (!expanded_arg)
			{
				printf("Erro ao expandir variáveis no argumento: %s\n", cmd->args[i]);
				return;
			}
			free(cmd->args[i]);
			cmd->args[i] = expanded_arg;
		}
		i++;
	}
}

static void	execute_child(t_cmd_node *cmd)
{
	setup_child_signals();
	if (cmd->in_fd != STDIN_FILENO)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->in_fd);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->out_fd);
	}
	execvp(cmd->args[0], cmd->args);
	fprintf(stderr, "minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(EXIT_FAILURE);
}

void execute_command(t_cmd_node *cmd, t_shell *shell)
{
    pid_t   pid;
    int     status;

    if (!cmd || !cmd->args || !cmd->args[0])
    {
        fprintf(stderr, "minishell: command not found\n");
        shell->exit_status = 127;
        return;
    }
	if (cmd->in_fd == -1)
    {
        fprintf(stderr, "minishell: No such file or directory\n");
        shell->exit_status = 1;
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        setup_child_signals();
        if (is_builtin(cmd))
            execute_builtin(cmd, shell);
        else
            execute_child(cmd);
    }
    else
    {
        waitpid(pid, &status, 0);
        shell->exit_status = WEXITSTATUS(status);
    }
}

void execute_pipeline(t_cmd_list *cmd_list, t_shell *shell)
{
    pid_t *pids;
    int pipefd[2], prev_pipe_in = -1, i = 0, valid_cmds = 0;
    t_cmd_node *cmd = cmd_list->head;

    if (!cmd)
        return;
    pids = malloc(sizeof(pid_t) * cmd_list->size);
    if (!pids)
        exit_with_error("Failed to allocate memory for pids");
    while (cmd)
    {
        if (!cmd->args || !cmd->args[0])
        {
            cmd = cmd->next;
            continue;
        }
        if (cmd->next && pipe(pipefd) == -1)
        {
            perror("pipe");
            free(pids);
            return;
        }
        pid_t pid = fork();
        if (pid == 0)
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

            if (!cmd->args || !cmd->args[0])
            {
                fprintf(stderr, "minishell: invalid command\n");
                exit(1);
            }  
            execvp(cmd->args[0], cmd->args);
            perror("execvp");
            exit(1);
        }
        else if (pid > 0)
        {
            pids[i++] = pid;
            valid_cmds++;
        }
        if (prev_pipe_in != -1)
            close(prev_pipe_in);
        if (cmd->next)
            close(pipefd[1]);
        prev_pipe_in = cmd->next ? pipefd[0] : -1;
        cmd = cmd->next;
    }
    int j = 0, status;
    while (j < i)
    {
        waitpid(pids[j], &status, 0);
        if (j == i - 1)
        {
            if (WIFEXITED(status))
                shell->exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                shell->exit_status = 128 + WTERMSIG(status);
        }
        j++;
    }
    free(pids);
}
