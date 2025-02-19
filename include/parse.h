/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:22:54 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/19 13:29:24 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

typedef struct s_redirection t_redirection;

typedef struct s_cmd_node
{
    char            *cmd;
    char            **args;
    t_redirection   *redirects;
    struct s_cmd_node *left;
    struct s_cmd_node *right;
} t_cmd_node;

typedef struct s_cmd_tree
{
    t_cmd_node  *root;
} t_cmd_tree;

typedef struct s_cmd_list
{
    t_token         *cmd;
    struct s_cmd_list *next;
} t_cmd_list;

t_cmd_tree	*parse_input(char *input_line);
t_cmd_list *create_cmd_list_node(void);
void add_token_to_cmd(t_cmd_list *list, t_token *token);
t_cmd_list *split_by_pipes(t_token *tokens);
t_cmd_list *process_pipe_tokens(t_token *tokens, t_cmd_list *current_cmd,
                               t_cmd_list *commands);
int add_to_commands(t_cmd_list **commands, t_cmd_list *new_cmd);
t_cmd_list *finalize_pipe_split(t_cmd_list *commands, t_cmd_list *current_cmd);
t_cmd_list *handle_pipe_error(t_cmd_list *commands, t_cmd_list *current_cmd);

#endif