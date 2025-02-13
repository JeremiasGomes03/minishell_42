/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_split_by_pipes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:10:31 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/13 16:40:27 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// estrutura auxiliar para construir a lista de comandos
t_cmd_list *create_cmd_list_node(void)
{
    t_cmd_list *new_list;

    new_list = malloc(sizeof(t_cmd_list));
    if (!new_list)
        return (NULL);
    new_list->cmd = NULL;
    new_list->next = NULL;
    return (new_list);
}

// add token a lista de comandos
void add_token_to_cmd(t_cmd_list *list, t_token *token)
{
    t_cmd_list *current;
    t_cmd_list *new_node;

    if (!list->cmd)
    {
        list->cmd = token;
        return ;
    }
    new_node = create_cmd_list_node();
    if (!new_node)
        return ;
    new_node->cmd = token;
    current = list;
    while (current->next)
        current = current->next;
    current->next = new_node;
}

// separa comandos por pipes
t_cmd_list *split_by_pipes(t_token *tokens)
{
    t_cmd_list *commands;
    t_cmd_list *current_cmd;
    int i;

    if (!tokens)
        return (NULL);
    commands = NULL;
    current_cmd = create_cmd_list_node();
    if (!current_cmd)
        return (NULL);
    return (process_pipe_tokens(tokens, current_cmd, commands));
}

// processa os tokens separados por pipes
t_cmd_list *process_pipe_tokens(t_token *tokens, t_cmd_list *current_cmd,
                               t_cmd_list *commands)
{
    int i;

    i = 0;
    while (tokens[i].value)
    {
        if (tokens[i].type == PIPE_TOKEN)
        {
            if (!current_cmd->cmd)
                return (handle_pipe_error(commands, current_cmd));
            if (!add_to_commands(&commands, current_cmd))
                return (handle_pipe_error(commands, current_cmd));
            current_cmd = create_cmd_list_node();
            if (!current_cmd)
                return (handle_pipe_error(commands, NULL));
        }
        else
            add_token_to_cmd(current_cmd, &tokens[i]);
        i++;
    }
    return (finalize_pipe_split(commands, current_cmd));
}

// add comando a lista de comando
int add_to_commands(t_cmd_list **commands, t_cmd_list *new_cmd)
{
    t_cmd_list *current;

    if (!*commands)
    {
        *commands = new_cmd;
        return (1);
    }
    current = *commands;
    while (current->next)
        current = current->next;
    current->next = new_cmd;
    return (1);
}

//finaliza os pipes
t_cmd_list *finalize_pipe_split(t_cmd_list *commands, t_cmd_list *current_cmd)
{
    if (!current_cmd->cmd)
    {
        free(current_cmd);
        if (!commands)
            return (NULL);
        return (commands);
    }
    if (!add_to_commands(&commands, current_cmd))
        return (handle_pipe_error(commands, current_cmd));
    return (commands);
}

t_cmd_list *handle_pipe_error(t_cmd_list *commands, t_cmd_list *current_cmd)
{
    if (current_cmd)
        free(current_cmd);
    if (commands)
        free_commands(commands);
    return (NULL);
}