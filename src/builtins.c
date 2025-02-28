/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/27 21:31:27 by jeremias         ###   ########.fr       */
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

    // Se não houver argumento, muda para o diretório HOME
    if (!cmd->args[1])
        dir = getenv("HOME"); // Obtém o diretório HOME
    else
        dir = cmd->args[1]; // Usa o diretório fornecido

    // Tenta mudar de diretório
    if (chdir(dir) == -1)
        perror("cd"); // Exibe erro se não for possível mudar de diretório
}
