/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:18:05 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/22 16:57:08 by jeremias         ###   ########.fr       */
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

t_token *create_token(char *value, t_token_type type) 
{
    t_token *new_token;
    
    if (!value)
        return NULL;
        
    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return NULL;
    new_token->value = ft_strdup(value);
    if (!new_token->value)
    {
        free(new_token);
        return (NULL);
    }
    new_token->type = type;
    new_token->next = NULL;
    return (new_token);
}

t_token *add_token(t_token *tokens, char *value, t_token_type type)
{
    t_token *new_token;
    t_token *current;

    if (!value)
        return tokens;
        
    new_token = create_token(value, type);
    if (!new_token) {
        return tokens;
    }
    if (!tokens) {
        return new_token;
    }
    current = tokens;
    while (current->next) {
        current = current->next;
    }
    current->next = new_token;
    return tokens;
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

void free_all_tokens(t_tokenizer *tokenizer) 
{
    t_token *current;
    t_token *next;

    if (!tokenizer || !tokenizer->tokens)
        return;     
    if (tokenizer->input)
        free(tokenizer->input);    
    current = tokenizer->tokens;
    while (current) {
        next = current->next;
        free_token(current);
        current = next;
    }
    tokenizer->tokens = NULL;
}
