/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:04:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/27 12:11:17 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd_list *parse_tokens(t_token *tokens)
{
    t_cmd_list  *cmd_list;
    t_cmd_node  *current;

    cmd_list = malloc(sizeof(t_cmd_list));
    if (!cmd_list)
        exit_with_error("Failed to allocate memory");
    cmd_list->head = NULL;
    cmd_list->tail = NULL;

    while (tokens && tokens->type != TOKEN_EOF)
    {
        current = parse_command(&tokens); // Usa parse_command para processar tokens até o próximo PIPE ou EOF
        append_command(cmd_list, current);

        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next; // Avança para o próximo comando após o pipe
    }
    return (cmd_list);
}


t_cmd_node *parse_command(t_token **tokens)
{
    t_cmd_node *cmd = create_cmd_node();

    while (*tokens && (*tokens)->type != TOKEN_PIPE && (*tokens)->type != TOKEN_EOF)
    {
        if (is_redirection((*tokens)->type))
            parse_redirection(cmd, tokens); 
        else
            add_arg_to_cmd(cmd, (*tokens)->value);
        *tokens = (*tokens)->next;
    }
    return cmd;
}


int is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

void append_command(t_cmd_list *cmd_list, t_cmd_node *cmd)
{
    if (!cmd_list->head)
    {
        cmd_list->head = cmd;
        cmd_list->tail = cmd;
    }
    else
    {
        cmd_list->tail->next = cmd;
        cmd_list->tail = cmd;
    }
}