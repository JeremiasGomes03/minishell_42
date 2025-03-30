/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:56:03 by jerda-si          #+#    #+#             */
/*   Updated: 2025/03/29 17:21:02 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;

	tokens = NULL;
	while (*input)
	{
		if (is_space(*input))
			input++;
		else if (is_operator(*input))
			handle_operator(&input, &tokens);
		else if (is_quote(*input))
		{
			handle_quotes(&input, &tokens, *input);
			if (!input)
				return (free_tokens(&tokens), NULL);
		}
		else
		{
			handle_word(&input, &tokens);
			handle_env_vars(&input, &tokens);
		}
	}
	if (tokens)
		add_token(&tokens, NULL, TOKEN_EOF, NO_QUOTES);
	return (tokens);
}

void	add_token(t_token **tokens,
	char *value, t_token_type type, t_quote_type quote_type)
{
	t_token		*new;
	t_token		*last;

	new = malloc(sizeof(t_token));
	if (!new)
		exit_with_error("Failed to allocate memory");
	new->value = value;
	new->type = type;
	new->quote_type = quote_type;
	new->next = NULL;
	if (!*tokens)
		*tokens = new;
	else
	{
		last = *tokens;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

void	handle_env_vars(char **input, t_token **tokens)
{
	char	*var_name;
	char	*var_value;
	int		i;

	i = 0;
	if ((*input)[0] == '$' && (*tokens)->quote_type != SINGLE_QUOTES)
	{
		(*input)++;
		while ((*input)[i] && (ft_isalnum((*input)[i]) || (*input)[i] == '_'))
			i++;
		var_name = ft_substr(*input, 0, i);
		var_value = getenv(var_name);
		if (var_value)
			add_token(tokens, ft_strdup(var_value), TOKEN_WORD, NO_QUOTES);
		free(var_name);
		*input += i;
	}
}
