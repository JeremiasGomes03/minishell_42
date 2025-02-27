/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:10:28 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/20 16:11:26 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_single_command(t_cmd_node *cmd)
{
    pid_t pid;
    char **env = get_env();

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
    {
        if (cmd->redirects)
            apply_redirections(cmd->redirects);
        
        execvp(cmd->cmd, cmd->args);
        perror("execvp");
        exit(1);
    }
    else
        waitpid(pid, NULL, 0);
}

void execute_commands(t_cmd_tree *cmd_tree)
{
    if (!cmd_tree || !cmd_tree->root)
        return;

    t_cmd_node *current = cmd_tree->root;

    while (current)
    {
        execute_single_command(current);
        current = current->next;
    }
}
