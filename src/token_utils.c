/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:18:05 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/22 18:09:34 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/token.h"

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

int is_regular_char(char c)
{
    return !is_space(c) && !is_operator(c) && !is_quote(c);
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
