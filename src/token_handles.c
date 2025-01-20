/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:01:42 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/20 19:02:25 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/token.h"

void handle_operator(t_tokenizer *tokenizer)
{
    char current_char = tokenizer->input[tokenizer->position];
    char next_char = tokenizer->input[tokenizer->position + 1];
    char operator_token[3] = {0};

    if ((current_char == '>' && next_char == '>') || 
        (current_char == '<' && next_char == '<'))
	{
        operator_token[0] = current_char;
        operator_token[1] = next_char;
        tokenizer->tokens = add_token(tokenizer->tokens, operator_token, TOKEN_APPEND);
        tokenizer->position += 2;
    }
	else
	{
        operator_token[0] = current_char;
        t_token_type type = (current_char == '|') ? TOKEN_PIPE :
                            (current_char == '<') ? TOKEN_REDIR_IN :
                            (current_char == '>') ? TOKEN_REDIR_OUT : TOKEN_WORD;
        tokenizer->tokens = add_token(tokenizer->tokens, operator_token, type);
        tokenizer->position++;
    }
}

void handle_regular_char(t_tokenizer *tokenizer)
{
    char current_token[256] = {0};
    int token_length = 0;

    while (tokenizer->input[tokenizer->position] != '\0' &&
           !is_space(tokenizer->input[tokenizer->position]) &&
           !is_operator(tokenizer->input[tokenizer->position]) &&
           !is_quote(tokenizer->input[tokenizer->position]))
	{
        current_token[token_length++] = tokenizer->input[tokenizer->position];
        tokenizer->position++;
    }
    if (token_length > 0)
	{
        current_token[token_length] = '\0';
        tokenizer->tokens = add_token(tokenizer->tokens, current_token, TOKEN_WORD);
    }
}

void handle_space(t_tokenizer *tokenizer, char *current_token, int *token_length)
{

    if (*token_length > 0)
	{
        current_token[*token_length] = '\0';
        tokenizer->tokens = add_token(tokenizer->tokens, current_token, TOKEN_WORD);
        ft_bzero(current_token, *token_length); 
        *token_length = 0; 
	}
    tokenizer->position++;
}
