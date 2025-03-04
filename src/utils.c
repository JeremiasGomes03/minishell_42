/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:18:05 by jerda-si          #+#    #+#             */
/*   Updated: 2025/03/03 23:48:02 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void exit_with_error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int validate_syntax(t_token *tokens)
{
    t_token *current = tokens;

    while (current)
    {
        if (current->type == TOKEN_PIPE)
        {
            if (!current->next || current->next->type == TOKEN_PIPE)
            {
                printf("Syntax error near unexpected token `|'\n");
                return (0);
            }
        }
        else if (is_redirection(current->type))
        {
            if (!current->next || current->next->type != TOKEN_WORD)
            {
                printf("Syntax error near unexpected token `%s'\n", current->value);
                return (0);
            }
        }
        current = current->next;
    }
    return (1);
}
