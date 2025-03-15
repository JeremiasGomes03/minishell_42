/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:08:12 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/14 21:56:42 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void parse_redirection(t_cmd_node *cmd, t_token **tokens, t_shell *shell)
{
    t_token_type type = (*tokens)->type;

    if (type == TOKEN_HEREDOC)
    {
        if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
            exit_with_error("Heredoc without delimiter");
        
        t_heredoc hd_data = {
            .delimiter = (*tokens)->next->value,
            .quote_type = (*tokens)->next->quote_type
        };
        cmd->in_fd = process_heredoc(&hd_data, shell);
        *tokens = (*tokens)->next->next;
    }
    else if (type == TOKEN_REDIR_IN)
    {
        if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
            exit_with_error("Redirection without file");
        cmd->in_fd = open((*tokens)->next->value, O_RDONLY);
        if (cmd->in_fd == -1)
            exit_with_error("Failed to open input file");
        *tokens = (*tokens)->next->next;
    }
    else if (type == TOKEN_REDIR_OUT)
    {
        if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
            exit_with_error("Redirection without file");
        cmd->out_fd = open((*tokens)->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (cmd->out_fd == -1)
            exit_with_error("Failed to open output file");
        *tokens = (*tokens)->next->next;
    }
    else if (type == TOKEN_APPEND)
    {
        if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
            exit_with_error("Append without file");
        cmd->out_fd = open((*tokens)->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (cmd->out_fd == -1)
            exit_with_error("Failed to open append file");
        *tokens = (*tokens)->next->next;
    }
}