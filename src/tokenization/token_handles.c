/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:01:42 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/27 19:31:43 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

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
