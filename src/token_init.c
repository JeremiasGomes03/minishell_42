/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:56:03 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/20 19:45:53 by jerda-si         ###   ########.fr       */
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

t_tokenizer  *tokenization_loop(char *input)
{
	int	i;
	t_tokenizer tokenizer = {input, 0, 0, NULL};
    char current_token[256] = {0};
    int token_length = 0;
	
	
	i = 0;
	if (token_length > 0)
	{
        current_token[token_length] = '\0';
        tokenizer.tokens = add_token(tokenizer.tokens, current_token, TOKEN_WORD);
    }
	while (input[i] != '\0')
	{
		if (is_quote(input[i]))
			handle_quoted_content(input, &i);
		else if (is_space(input[i]))
			handle_space(&tokenizer, current_token, &token_length);
		else if (is_operator(input[i]))
			handle_operator(&tokenizer);
		else if (is_regular_char(input[i]))
			handle_regular_char(&tokenizer);
		i++;
	}
	return (NULL);
}

bool matching_quote(char c)
{
	char current_quote = '\0';
    return (current_quote != '\0' && current_quote == c);
}

void start_quote_state(char quote)
{
	char current_quote = '\0';
    if (current_quote == '\0')
        current_quote = quote;
}

void end_quote_state()
{
	char current_quote = '\0';
    if (current_quote != '\0')
        current_quote = '\0';
}

void handle_quoted_content(const char *input, int *i)
{
	

    if (input[*i] == '"' || input[*i] == '\'')
	{
        if (matching_quote(input[*i]))
		{
            end_quote_state();
        }
		else
		{ 
            
            start_quote_state(input[*i]);
        }
    }
	else
	{
        add_to_current_token(input[*i]);
    }
}

void add_to_current_token(char c)
{
	t_tokenizer *tokenizer;
	
	tokenizer = NULL;
    if (tokenizer->tokens == NULL)
	{
        tokenizer->tokens = create_token("", TOKEN_WORD);
    }
    size_t current_length = ft_strlen(tokenizer->tokens->value);
    size_t new_length = current_length + 2;
    tokenizer->tokens->value = realloc(tokenizer->tokens->value, new_length);
    if (tokenizer->tokens->value == NULL)
	{

        perror("Failed to realloc memory for token");
        exit(EXIT_FAILURE);
    }
    tokenizer->tokens->value[current_length] = c;
    tokenizer->tokens->value[current_length + 1] = '\0';
}

