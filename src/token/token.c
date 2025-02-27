/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:56:03 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/27 13:17:11 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void add_token(t_token **tokens, char *value, t_token_type type)
{
    t_token *new = malloc(sizeof(t_token));
    t_token *last;

    if (!new)
        exit_with_error("Failed to allocate memory");
    new->value = value ? strdup(value) : NULL;
    new->type = type;
    new->next = NULL;
    if (!*tokens)
    {
        *tokens = new;
        return;
    }
    last = *tokens;
    while (last->next)
        last = last->next;
    last->next = new;
}

t_token *tokenize_input(char *input)
{
    t_token *tokens = NULL;

    while (*input)
    {
        if (is_space(*input))
            input++;
        else if (is_operator(*input))
            handle_operator(&input, &tokens);
        else if (is_quote(*input))
            handle_quotes(&input, &tokens, *input);
        else
            handle_word(&input, &tokens);
    }

    // Não adicionar TOKEN_EOF desnecessário ao final da entrada
    return tokens;
}

void exit_with_error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
