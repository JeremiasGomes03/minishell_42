/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:08:12 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/27 21:09:12 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void parse_redirection(t_cmd_node *cmd, t_token **tokens)
{
    t_token_type type = (*tokens)->type;
    char        *file;

    if (type == TOKEN_HEREDOC)
    {
        file = (*tokens)->next->value;
        cmd->in_fd = process_heredoc(file);
        *tokens = (*tokens)->next->next;
    }
    else if (type == TOKEN_REDIR_IN)
    {
        file = (*tokens)->next->value;
        cmd->in_fd = open(file, O_RDONLY);
        *tokens = (*tokens)->next->next;
    }
    else if (type == TOKEN_REDIR_OUT)
    {
        file = (*tokens)->next->value;
        cmd->out_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        *tokens = (*tokens)->next->next;
    }
    else if (type == TOKEN_APPEND)
    {
        file = (*tokens)->next->value;
        cmd->out_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        *tokens = (*tokens)->next->next;
    }
}