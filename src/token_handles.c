/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:01:42 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/22 16:54:39 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/token.h"

void handle_operator(t_tokenizer *tokenizer)
{
    if (!tokenizer || !tokenizer->input)
        return;
 
    char current_char = tokenizer->input[tokenizer->position];
    char next_char = tokenizer->input[tokenizer->position + 1];
    char operator_token[3] = {0};

    if (current_char == '>' && next_char == '>')
    {
        operator_token[0] = current_char;
        operator_token[1] = next_char;
        tokenizer->tokens = add_token(tokenizer->tokens, operator_token, TOKEN_APPEND);
        tokenizer->position++;
    }
    else
    {
        operator_token[0] = current_char;
        operator_token[1] = '\0';
        t_token_type type = (current_char == '|') ? TOKEN_PIPE :
                           (current_char == '<') ? TOKEN_REDIR_IN :
                           (current_char == '>') ? TOKEN_REDIR_OUT : TOKEN_WORD;
        tokenizer->tokens = add_token(tokenizer->tokens, operator_token, type);
    }
    tokenizer->position++;
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
    if (!tokenizer || !current_token || !token_length)
        return;

    if (*token_length > 0)
    {
        current_token[*token_length] = '\0';
        tokenizer->tokens = add_token(tokenizer->tokens, current_token, TOKEN_WORD);
        *token_length = 0;
    }
    tokenizer->position++;
}
