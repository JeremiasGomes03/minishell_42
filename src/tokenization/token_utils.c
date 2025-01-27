/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:18:05 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/27 19:33:22 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int is_quote(char c)
{
    return (c == '\'' || c == '\"');
}

int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>' ||
            c == '$' || c == '\\' ||
            c == '#' || c == '~');
}

void free_token(t_token *token)
{
    if (token)
    {
        if (token->value)
            free(token->value);
        free(token);
    }
}
