/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:25 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/27 21:35:59 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void execute_command(t_cmd_node *cmd)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) // Erro no fork
    {
        perror("fork");
        return;
    }
    else if (pid == 0) // Processo filho
    {
        if (cmd->in_fd != STDIN_FILENO)
        {
            if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(cmd->in_fd); // Fecha o file descriptor original
        }
        if (cmd->out_fd != STDOUT_FILENO)
        {
            if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(cmd->out_fd); // Fecha o file descriptor original
        }
        execvp(cmd->args[0], cmd->args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else // Processo pai
    {
        waitpid(pid, &status, 0);
        if (cmd->in_fd != STDIN_FILENO)
            close(cmd->in_fd);
        if (cmd->out_fd != STDOUT_FILENO)
            close(cmd->out_fd);
    }
}

void execute_pipeline(t_cmd_list *cmd_list)
{
    int pipefd[2];
    t_cmd_node *cmd = cmd_list->head;
    int prev_pipe_in = -1;

    while (cmd)
    {
        if (cmd->next)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                return;
            }
        }
        if (prev_pipe_in != -1)
            cmd->in_fd = prev_pipe_in;
        if (cmd->next)
            cmd->out_fd = pipefd[1];
        execute_command(cmd);
        if (prev_pipe_in != -1)
            close(prev_pipe_in);
        if (cmd->next)
        {
            close(pipefd[1]);
            prev_pipe_in = pipefd[0];
        }

        cmd = cmd->next;
    }
    if (prev_pipe_in != -1)
        close(prev_pipe_in);
}