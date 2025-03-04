/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:25 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/03 23:45:14 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void execute_command(t_cmd_node *cmd, t_shell *shell)
{
    pid_t pid;
    int status;

    int i = 0;
    while (cmd->args && cmd->args[i])
    {
        // Debug: Mostra o argumento atual antes de expandir
        printf("Argumento antes de expandir: %s\n", cmd->args[i]);

        // Verifica se o argumento contém uma variável para expandir
        if (ft_strchr(cmd->args[i], '$'))
        {
            char *expanded_arg = expand_variables(cmd->args[i], shell);
            if (!expanded_arg)
            {
                printf("Erro ao expandir variáveis no argumento: %s\n", cmd->args[i]);
                return;
            }
            free(cmd->args[i]);
            cmd->args[i] = expanded_arg;
        }

        // Debug: Mostra o comando expandido
        printf("Comando expandido: %s\n", cmd->args[i]);

        i++;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
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

        // Debug: Comando final antes de executar
        printf("Executando comando: %s\n", cmd->args[0]);

        execvp(cmd->args[0], cmd->args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(pid, &status, 0);
        shell->exit_status = WEXITSTATUS(status);
    }
}

void execute_pipeline(t_cmd_list *cmd_list, t_shell *shell)
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

        execute_command(cmd, shell);

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
