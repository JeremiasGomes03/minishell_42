/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:56:03 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/31 15:12:52 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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


t_tokenizer	*tokenization_loop(char *input)
{
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

t_token	*create_token(char *value, t_token_type type) 
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

t_token	*add_token(t_token *tokens, char *value, t_token_type type)
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
