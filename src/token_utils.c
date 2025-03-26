/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 21:40:09 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/26 19:07:43 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

void free_tokens(t_token *tokens)
{
    t_token *temp;
    while (tokens)
	{
        temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
}

char	*get_quoted_literal(char **input, char quote)
{
	char	*start;
	char	*literal;
	size_t	len;

	start = *input;
	(*input)++;
	while (**input && **input != quote)
		(*input)++;
	if (**input != quote)
		return (NULL);
	len = *input - start - 1;
	literal = ft_substr(start + 1, 0, len);
	(*input)++;
	return (literal);
}
