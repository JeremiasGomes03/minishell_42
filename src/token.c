/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:56:03 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/28 17:46:03 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

    if (tokens)
        add_token(&tokens, NULL, TOKEN_EOF);

    return tokens;
}

void add_token(t_token **tokens, char *value, t_token_type type)
{
    t_token *new = malloc(sizeof(t_token));
    t_token *last;

    if (!new)
        exit_with_error("Failed to allocate memory");
    new->value = value ? ft_strdup(value) : NULL;
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

void handle_env_vars(char **input, t_token **tokens)
{
    char    *var_name;
    char    *var_value;
    int     i = 0;

    if ((*input)[0] == '$')
    {
        (*input)++;
        while ((*input)[i] && (ft_isalnum((*input)[i]) || (*input)[i] == '_'))
            i++;
        var_name = ft_substr(*input, 0, i);
        var_value = getenv(var_name);
        if (var_value)
            add_token(tokens, ft_strdup(var_value), TOKEN_WORD);
        free(var_name);
        *input += i;
    }
}
