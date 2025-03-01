/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 19:30:48 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/27 22:19:30 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/libft.h" 

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_EOF
} t_token_type;

typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
} t_token;

typedef struct s_cmd_node
{
    char                **args;
    int                 in_fd;
    int                 out_fd;
    struct s_cmd_node   *next;
} t_cmd_node;

typedef struct s_cmd_list
{
    t_cmd_node  *head;
    t_cmd_node  *tail;
} t_cmd_list;


// Tokenização
t_token     *tokenize_input(char *input);
void        handle_operator(char **input, t_token **tokens);
void        handle_word(char **input, t_token **tokens);
void        handle_quotes(char **input, t_token **tokens, char quote);
void        add_token(t_token **tokens, char *value, t_token_type type);
void        handle_env_vars(char **input, t_token **tokens);
char        *ft_strjoin_char(char *s1, char c);

// Tokenização Utils
int         is_space(char c);
int         is_operator(char c);
int         is_quote(char c);
void        free_tokens(t_token *tokens);

// Parsing
t_cmd_list  *parse_tokens(t_token *tokens);
t_cmd_node  *parse_command(t_token **tokens);
void        parse_redirection(t_cmd_node *cmd, t_token **tokens);
int         is_redirection(t_token_type type);
void        append_command(t_cmd_list *cmd_list, t_cmd_node *cmd);
int         process_heredoc(char *delimiter);
char        *ft_strjoin_with_free(char *s1, char *s2, int free_s1);

// Parsing Utils
void        append_command(t_cmd_list *cmd_list, t_cmd_node *cmd);
void        free_cmd_list(t_cmd_list *cmd_list);
t_cmd_node *create_cmd_node(void);
void        add_arg_to_cmd(t_cmd_node *cmd, char *arg);
int         ft_isnumeric(char *str);

// Execução
void        execute_command(t_cmd_node *cmd);
void        execute_pipeline(t_cmd_list *cmd_list);

// Builtins
void        builtin_echo(t_cmd_node *cmd);
void        builtin_cd(t_cmd_node *cmd);
void        builtin_exit(t_cmd_node *cmd);

// Sinais
void        handle_signal(int sig);
void        setup_signals(void);
void        handle_eof(void);

// Utils
void        exit_with_error(char *msg);
void        free_cmd_list(t_cmd_list *cmd_list);
t_cmd_node  *create_cmd_node(void);
void        add_arg_to_cmd(t_cmd_node *cmd, char *arg);
int         validate_syntax(t_token *tokens);
char        *expand_variables(char *input);
void        expand_command_vars(t_cmd_node *cmd);

#endif