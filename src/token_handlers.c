/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamachad <lamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:01:42 by jerda-si          #+#    #+#             */
/*   Updated: 2025/03/05 20:40:13 by lamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

void	handle_operator(char **input, t_token **tokens)
{
	char	*value;

	if ((*input)[0] == '|')
	{
		value = ft_strdup("|");
		add_token(tokens, value, TOKEN_PIPE);
		(*input)++;
	}
	else if ((*input)[0] == '<' && (*input)[1] == '<')
	{
		value = ft_strdup("<<");
		add_token(tokens, value, TOKEN_HEREDOC);
		(*input) += 2;
	}
	else if ((*input)[0] == '>' && (*input)[1] == '>')
	{
		value = ft_strdup(">>");
		add_token(tokens, value, TOKEN_APPEND);
		(*input) += 2;
	}
	else if ((*input)[0] == '<')
	{
		value = ft_strdup("<");
		add_token(tokens, value, TOKEN_REDIR_IN);
		(*input)++;
	}
	else if ((*input)[0] == '>')
	{
		value = ft_strdup(">");
		add_token(tokens, value, TOKEN_REDIR_OUT);
		(*input)++;
	}
}

void	handle_word(char **input, t_token **tokens)
{
	char	*value;
	int		i;

	i = 0;
	while ((*input)[i] && !is_space((*input)[i]) && !is_operator((*input)[i]) && !is_quote((*input)[i]))
		i++;
	value = ft_substr(*input, 0, i);
	add_token(tokens, value, TOKEN_WORD);
	*input += i;
}

void handle_quotes(char **input, t_token **tokens, char quote)
{
    char *value;
    int i = 1;

    (*input)++;
    while ((*input)[i] && (*input)[i] != quote)
        i++;
    if ((*input)[i] == quote)
    {
        value = ft_substr(*input, 0, i);
        add_token(tokens, value, TOKEN_WORD);
        *input += i + 1;
    }
    else
    {
        exit_with_error("Unclosed quote");
    }
}




