/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:01:42 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/22 18:05:54 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/token.h"

void handle_operator(t_tokenizer *tokenizer, int *i)
{
    char operator_token[3] = {0};
    char current_char;
    char next_char;

    current_char = tokenizer->input[*i];
    next_char = tokenizer->input[*i + 1];
    if (current_char == '>' && next_char == '>') {
        operator_token[0] = current_char;
        operator_token[1] = next_char;
        tokenizer->tokens = add_token(tokenizer->tokens, operator_token, TOKEN_APPEND);
        (*i)++;
    } else {
        operator_token[0] = current_char;
        operator_token[1] = '\0';

        t_token_type type = (current_char == '|') ? TOKEN_PIPE :
                            (current_char == '<') ? TOKEN_REDIR_IN :
                            (current_char == '>') ? TOKEN_REDIR_OUT : TOKEN_WORD;

        tokenizer->tokens = add_token(tokenizer->tokens, operator_token, type);
    }
    (*i)++;
}

void handle_regular_char(t_tokenizer *tokenizer)
{
    if (!tokenizer || !tokenizer->input)
        return;

    char current_token[256] = {0};
    int token_length = 0;

    while (tokenizer->position < (int)ft_strlen(tokenizer->input) && 
           tokenizer->input[tokenizer->position] &&
           !is_space(tokenizer->input[tokenizer->position]) &&
           !is_operator(tokenizer->input[tokenizer->position]) &&
           !is_quote(tokenizer->input[tokenizer->position]) &&
           token_length < 255)
    {
        current_token[token_length++] = tokenizer->input[tokenizer->position++];
    }

    if (token_length > 0)
    {
        current_token[token_length] = '\0';
        tokenizer->tokens = add_token(tokenizer->tokens, current_token, TOKEN_WORD);
    }
}

void handle_space(t_tokenizer *tokenizer, char *current_token, int *token_length)
{
    if (*token_length > 0) {
        current_token[*token_length] = '\0';
        tokenizer->tokens = add_token(tokenizer->tokens, current_token, TOKEN_WORD);
        *token_length = 0;
    }
}

void handle_quoted_content(const char *input, int *i, char *current_token, int *token_length, char *current_quote)
{
    char quote_type = input[*i];
    
    if (*current_quote == '\0') {
        *current_quote = quote_type;
    } else if (*current_quote == quote_type) {
        *current_quote = '\0';
    }
    
    current_token[*token_length] = input[*i];
    (*token_length)++;
    (*i)++;
}
