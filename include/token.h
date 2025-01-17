/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:43:24 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/17 18:54:11 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H

# define TOKEN_H

#include <stdio.h>
# include "../lib/libft/libft.h"

typedef enum e_token_type
{
    TOKEN_WORD,      // Palavras normais e argumentos
    TOKEN_PIPE,      // |
    TOKEN_REDIR_IN,  // <
    TOKEN_REDIR_OUT, // >
    TOKEN_APPEND,    // >>
    TOKEN_QUOTE,     // Conteúdo entre aspas simples
    TOKEN_DQUOTE     // Conteúdo entre aspas duplas
} t_token_type;

typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
} t_token;

typedef struct s_tokenizer {
    char    *input;     // string a ser processada
    int     position;   // posição atual
    int     quote_state; // estado das aspas
    t_token *tokens;    // lista de tokens
} t_tokenizer;

int		is_space(char c);
int		is_quote(char c);
int		is_operator(char c);
int		is_special_char(char c);
void	free_token(t_token *token);
void free_all_tokens(t_tokenizer *tokenizer);
t_token	*create_token(char *value, t_token_type type);
t_token	*add_token(t_token *tokens, char *value, t_token_type type);


#endif