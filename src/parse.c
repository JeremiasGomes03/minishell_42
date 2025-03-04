/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:04:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/04 16:40:53 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd_list *parse_tokens(t_token *tokens)
{
    t_cmd_list  *cmd_list;
    t_cmd_node  *current;

    cmd_list = malloc(sizeof(t_cmd_list));
    if (!cmd_list)
        exit_with_error("Failed to allocate memory");
    cmd_list->head = NULL;
    cmd_list->tail = NULL;

    while (tokens && tokens->type != TOKEN_EOF)
    {
        current = parse_command(&tokens);
        append_command(cmd_list, current);

        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
    }
    return cmd_list;
}

t_cmd_node *parse_command(t_token **tokens)
{
    t_cmd_node *cmd = create_cmd_node();

    while (*tokens && (*tokens)->type != TOKEN_PIPE && (*tokens)->type != TOKEN_EOF)
    {
        if (is_redirection((*tokens)->type))
            parse_redirection(cmd, tokens);
        else
            add_arg_to_cmd(cmd, (*tokens)->value);
        *tokens = (*tokens)->next;
    }
    return cmd;
}


int process_heredoc(char *delimiter)
{
    char    *line;
    char    *content = NULL;
    int     fd;
    char    *temp_file = "/tmp/minishell_heredoc";

    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        content = ft_strjoin_with_free(content, line, 1);
        content = ft_strjoin_with_free(content, "\n", 1);
        free(line);
    }
    fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        free(content);
        exit_with_error("Failed to create heredoc file");
    }
    write(fd, content, ft_strlen(content));
    close(fd);
    free(content);
    fd = open(temp_file, O_RDONLY);
    if (fd == -1)
        exit_with_error("Failed to open heredoc file");
    unlink(temp_file); 
    return (fd);
}

