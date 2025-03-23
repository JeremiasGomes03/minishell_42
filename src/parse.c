/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:04:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/23 16:31:28 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void free_cmd_node(t_cmd_node *cmd)
{
    if (cmd)
    {
        if (cmd->args)
            ft_free_array(cmd->args);
        if (cmd->in_fd != STDIN_FILENO && cmd->in_fd != -1)
            close(cmd->in_fd);
        if (cmd->out_fd != STDOUT_FILENO && cmd->out_fd != -1)
            close(cmd->out_fd);
        free(cmd);
    }
}

t_cmd_list *parse_tokens(t_token *tokens, t_shell *shell)
{
    t_cmd_list *cmd_list;
    t_cmd_node *current;
    t_token *original_tokens = tokens;

    if (tokens && tokens->type == TOKEN_PIPE)
    {
        fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
        free_tokens(original_tokens);
        shell->exit_status = 258;
        return (NULL);
    }
    cmd_list = malloc(sizeof(t_cmd_list));
    if (!cmd_list)
        exit_with_error("Failed to allocate memory");
    cmd_list->head = NULL;
    cmd_list->tail = NULL;
    while (tokens && tokens->type != TOKEN_EOF)
    {
        current = parse_command(&tokens, shell);
        if (!current)
        {
            free_tokens(original_tokens);
            free_cmd_list(cmd_list);
            shell->exit_status = 258;
            return (NULL);
        }
        append_command(cmd_list, current);
        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
    }
    return (cmd_list);
}


t_cmd_node *parse_command(t_token **tokens, t_shell *shell)
{
    t_cmd_node *cmd;
	
    if (*tokens && is_redirection((*tokens)->type))
    {
        fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", (*tokens)->value);
        shell->exit_status = 258;
        return (NULL);
    }
    cmd = create_cmd_node();
    while (*tokens && (*tokens)->type != TOKEN_PIPE && (*tokens)->type != TOKEN_EOF)
    {
        if (is_redirection((*tokens)->type))
        {
            if (!parse_redirection(cmd, tokens, shell))
            {
                free_cmd_node(cmd);
                return (NULL);
            }
        }
        else
        {
            add_arg_to_cmd(cmd, (*tokens)->value);
            *tokens = (*tokens)->next;
        }
    }
    if (!cmd->args || !cmd->args[0])
    {
        fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
        shell->exit_status = 258;
        free_cmd_node(cmd);
        return (NULL);
    }
    if (cmd->in_fd == -1)
    {
        free_cmd_node(cmd);
        return (NULL);
    }
    return (cmd);
}

