/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:01:42 by jerda-si          #+#    #+#             */
/*   Updated: 2025/03/29 20:04:21 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_operator(char **input, t_token **tokens)
{
	char	*value;
	int		len;

	if ((*input)[0] == (*input)[1] && ((*input)[0] == '<'
		|| (*input)[0] == '>'))
		len = 2;
	else
		len = 1;
	value = ft_substr(*input, 0, len);
	if (!value)
		return ;
	if (**input == '|')
		add_token(tokens, value, TOKEN_PIPE, NO_QUOTES);
	else if (**input == '<' && len == 2)
		add_token(tokens, value, TOKEN_HEREDOC, NO_QUOTES);
	else if (**input == '>' && len == 2)
		add_token(tokens, value, TOKEN_APPEND, NO_QUOTES);
	else if (**input == '<')
		add_token(tokens, value, TOKEN_REDIR_IN, NO_QUOTES);
	else if (**input == '>')
		add_token(tokens, value, TOKEN_REDIR_OUT, NO_QUOTES);
	*input += len;
}

void	handle_word(char **input, t_token **tokens)
{
	char	*value;
	int		i;

	i = 0;
	while ((*input)[i] && !is_space((*input)[i])
		&& !is_operator((*input)[i]) && !is_quote((*input)[i]))
		i++;
	value = ft_substr(*input, 0, i);
	if (!value)
		exit(EXIT_FAILURE);
	add_token(tokens, value, TOKEN_WORD, NO_QUOTES);
	*input += i;
}

void	handle_quotes(char **input, t_token **tokens, char quote)
{
	char			*value;
	t_quote_type	quote_type;

	if (quote == '\'')
		quote_type = SINGLE_QUOTES;
	else
		quote_type = DOUBLE_QUOTES;
	value = get_quoted_literal(input, quote);
	if (!value)
	{
		printf("minishell: syntax error: unclosed quote\n");
		*input = NULL;
		return ;
	}
	add_token(tokens, value, TOKEN_WORD, quote_type);
}
