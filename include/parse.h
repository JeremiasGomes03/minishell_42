/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:22:54 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/05 17:41:31 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H

# define PARSE_H

# include <stdio.h>
# include "../lib/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

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

#endif