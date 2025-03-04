/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/28 17:05:04 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void builtin_echo(t_cmd_node *cmd)
{
    for (int i = 1; cmd->args[i]; i++)
    {
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1]) printf(" ");
    }
    printf("\n");
}

void builtin_cd(t_cmd_node *cmd)
{
    char *dir;

    if (!cmd->args[1])
        dir = getenv("HOME");
    else
        dir = cmd->args[1];
    if (chdir(dir) == -1)
        perror("cd");
}
