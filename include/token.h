/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:43:24 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/13 16:37:24 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H

# define TOKEN_H

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_QUOTE,
	TOKEN_DQUOTE
} t_token_type;

typedef struct s_token
{
	char            *value;
	t_token_type    type;
	struct s_token  *next;
} t_token;

typedef struct s_tokenizer {
	char    *input;
	int     position;
	int     quote_state;
	t_token *tokens;
} t_tokenizer;

int		is_space(char c);
int		is_quote(char c);
int		is_operator(char c);
void	free_token(t_token *token);
void free_all_tokens(t_tokenizer *tokenizer);
t_token	*create_token(char *value, t_token_type type);
t_token	*add_token(t_token *tokens, char *value, t_token_type type);
t_tokenizer  *init_tokenizer(char *input);
t_tokenizer  *tokenization_loop(char *input);
void handle_quoted_content(const char *input, int *i, char *current_token, int *token_length, char *current_quote);
void handle_space(t_tokenizer *tokenizer, char *current_token, int *token_length);
void handle_operator(t_tokenizer *tokenizer, int *i);

#endif