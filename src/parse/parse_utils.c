/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:06:00 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/27 12:05:50 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd_node *create_cmd_node(void)
{
    t_cmd_node *node;

    node = malloc(sizeof(t_cmd_node));
    if (!node)
        exit_with_error("Failed to allocate memory");
    node->args = malloc(sizeof(char *));
    if (!node->args)
        exit_with_error("Failed to allocate memory");
    node->args[0] = NULL;
    node->in_fd = STDIN_FILENO;
    node->out_fd = STDOUT_FILENO;
    node->next = NULL;
    return (node);
}

void free_cmd_list(t_cmd_list *cmd_list)
{
    t_cmd_node *current = cmd_list->head;
    t_cmd_node *next;

    while (current)
    {
        next = current->next;
        free(current->args);
        free(current);
        current = next;
    }
    free(cmd_list);
}

void add_arg_to_cmd(t_cmd_node *cmd, char *arg)
{
    int i;
    
    printf("Adding argument: %s to command\n", arg);
    i = 0;
    while (cmd->args && cmd->args[i])
        i++;
    cmd->args = realloc(cmd->args, (i + 2) * sizeof(char *));
    if (!cmd->args)
        exit_with_error("Failed to allocate memory");
    cmd->args[i] = arg;
    cmd->args[i + 1] = NULL;
}