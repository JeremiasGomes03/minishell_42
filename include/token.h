/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:43:24 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/27 13:12:34 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:43:24 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/26 16:18:50 by jeremias         ###   ########.fr       */
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
    TOKEN_HEREDOC,
    TOKEN_EOF,
    TOKEN_HEREDOC_DELIM
}   t_token_type;

typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
}   t_token;

// Tokenization
t_token     *tokenize_input(char *input);
void handle_operator(char **input, t_token **tokens);
void        handle_heredoc(char **input, t_token **tokens);
void        handle_quotes(char **input, t_token **tokens, char quote);
void        handle_word(char **input, t_token **tokens);
void add_token(t_token **tokens, char *value, t_token_type type);

char *read_heredoc_content(char *delimiter);

// Utils
int         is_space(char c);
int         is_operator(char c);
int         is_quote(char c);
void        free_tokens(t_token *tokens);

void exit_with_error(char *msg);

void handle_env_vars(char **input, t_token **tokens);
void handle_escaped_chars(char **input, t_token **tokens);
void handle_comments(char **input);

t_token *reverse_tokens(t_token *tokens);
void handle_heredoc_delim(char **input, t_token **tokens);

#endif