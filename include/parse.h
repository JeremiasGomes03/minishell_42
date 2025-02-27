/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:03:26 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/27 12:12:57 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_cmd_node
{
    char                **args;
    int                 in_fd;
    int                 out_fd;
    struct s_cmd_node   *next;
}   t_cmd_node;

typedef struct s_cmd_list
{
    t_cmd_node  *head;
    t_cmd_node  *tail;
}   t_cmd_list;

// Parsing
t_cmd_list  *parse_tokens(t_token *tokens);
t_cmd_node  *parse_command(t_token **tokens);
void        parse_redirection(t_cmd_node *cmd, t_token **tokens);

// Utils
t_cmd_node  *create_cmd_node(void);
void        free_cmd_list(t_cmd_list *cmd_list);

int process_heredoc(char *delimiter);
int validate_syntax(t_token *tokens);

void add_arg_to_cmd(t_cmd_node *cmd, char *arg);
int is_redirection(t_token_type type);
void append_command(t_cmd_list *cmd_list, t_cmd_node *cmd);

#endif  