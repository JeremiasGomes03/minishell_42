/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:56:03 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/22 18:03:41 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"

t_tokenizer  *init_tokenizer(char *input)
{
	t_tokenizer *tokenizer;
	
	if (!input)
		return (NULL);
	tokenizer = (t_tokenizer *)malloc(sizeof(t_tokenizer));
	if (!tokenizer)
		return (NULL);
	tokenizer->input = ft_strdup(input);
	if (!tokenizer->input)
	{
    	free(tokenizer);
    	return (NULL);
	}
	tokenizer->position = 0; 
    tokenizer->quote_state = 0; 
    tokenizer->tokens = NULL;
	return (tokenizer);
}


t_tokenizer *tokenization_loop(char *input) {
    t_tokenizer *tokenizer;
    int i = 0;
    char current_token[256] = {0};
    int token_length = 0;
    char current_quote = '\0';

    tokenizer = init_tokenizer(input);
    if (!tokenizer)
        return NULL;

    while (input[i] != '\0') {
        if (is_quote(input[i])) {
            handle_quoted_content(input, &i, current_token, &token_length, &current_quote);
        } else if (is_space(input[i]) && current_quote == '\0') {
            handle_space(tokenizer, current_token, &token_length);
            i++;
        } else if (is_operator(input[i]) && current_quote == '\0') {
            handle_operator(tokenizer, &i);
        } else {
            current_token[token_length++] = input[i];
            i++;
        }
    }

    if (token_length > 0) {
        current_token[token_length] = '\0';
        tokenizer->tokens = add_token(tokenizer->tokens, current_token, TOKEN_WORD);
    }
    return tokenizer;
}
