/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:08:12 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/27 13:05:00 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void parse_redirection(t_cmd_node *cmd, t_token **tokens)
{
    t_token_type type = (*tokens)->type;
    char        *file;

    if (type == TOKEN_HEREDOC)
    {
        file = (*tokens)->next->value; // O delimitador do heredoc
        cmd->in_fd = process_heredoc(file);
        *tokens = (*tokens)->next->next; // Avança dois tokens (<< e delimitador)
    }
    else if (type == TOKEN_REDIR_IN)
    {
        file = (*tokens)->next->value; // O nome do arquivo para redirecionamento de entrada
        cmd->in_fd = open(file, O_RDONLY);
        *tokens = (*tokens)->next->next; // Avança dois tokens (< e nome do arquivo)
    }
    else if (type == TOKEN_REDIR_OUT)
    {
        file = (*tokens)->next->value; // O nome do arquivo para redirecionamento de saída
        cmd->out_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        *tokens = (*tokens)->next->next; // Avança dois tokens (> e nome do arquivo)
    }
    else if (type == TOKEN_APPEND)
    {
        file = (*tokens)->next->value; // O nome do arquivo para redirecionamento de saída (append)
        cmd->out_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        *tokens = (*tokens)->next->next; // Avança dois tokens (>> e nome do arquivo)
    }
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
        content = ft_strjoin_with_free(content, line, 1); // Junta linhas mantendo \n
        content = ft_strjoin_with_free(content, "\n", 1);
        free(line);
    }

    // Escreve o conteúdo em um arquivo temporário
    fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, content, ft_strlen(content));
    close(fd);
    free(content);

    // Abre o arquivo para leitura e retorna o fd
    fd = open(temp_file, O_RDONLY);
    unlink(temp_file);
    return (fd);
}