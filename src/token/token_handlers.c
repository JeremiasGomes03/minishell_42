/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:01:42 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/27 13:19:57 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

void handle_operator(char **input, t_token **tokens)
{
    char *value;

    if ((*input)[0] == '|')
    {
        value = ft_strdup("|");
        add_token(tokens, value, TOKEN_PIPE);
        (*input)++;
    }
    else if ((*input)[0] == '<' && (*input)[1] == '<')  // Heredoc
    {
        value = ft_strdup("<<");
        add_token(tokens, value, TOKEN_HEREDOC);  // Usando TOKEN_HEREDOC para o operador
        (*input) += 2;

        // Tokeniza o delimitador, tratando-o como um TOKEN_HEREDOC_DELIM
        while (is_space(**input)) // Ignora espaços após o operador
            (*input)++;
        handle_word(input, tokens); // Captura o delimitador como um único TOKEN_WORD
    }
    else if ((*input)[0] == '>' && (*input)[1] == '>')
    {
        value = ft_strdup(">>");
        add_token(tokens, value, TOKEN_APPEND);
        (*input) += 2;
    }
    else if ((*input)[0] == '<')
    {
        value = ft_strdup("<");
        add_token(tokens, value, TOKEN_REDIR_IN);
        (*input)++;
    }
    else if ((*input)[0] == '>')
    {
        value = ft_strdup(">");
        add_token(tokens, value, TOKEN_REDIR_OUT);
        (*input)++;
    }
}



void handle_heredoc(char **input, t_token **tokens)
{
    char    *delim;
    char    *content;
    int     i = 0;

    // Avança além de '<<'
    *input += 2;

    // Captura o delimitador
    while ((*input)[i] && !is_space((*input)[i]) && !is_operator((*input)[i]))
        i++;
    delim = ft_substr(*input, 0, i);
    *input += i;

    // Ignora espaços após o delimitador
    while (is_space(**input))
        (*input)++;

    // Adiciona o delimitador como um token especial
    add_token(tokens, delim, TOKEN_HEREDOC_DELIM);  // Usando TOKEN_HEREDOC_DELIM para o delimitador

    // Captura o conteúdo do heredoc
    content = read_heredoc_content(delim);  // Implementada anteriormente
    free(delim);

    // Adiciona o conteúdo como um único token
    add_token(tokens, content, TOKEN_WORD);

    // Avança até o final do heredoc
    while (**input && **input != '\n')
        (*input)++;
}


void handle_heredoc_delim(char **input, t_token **tokens)
{
    char *value;
    int i = 0;

    while ((*input)[i] && !is_space((*input)[i]) && !is_operator((*input)[i]))
        i++;
    value = ft_substr(*input, 0, i);
    add_token(tokens, value, TOKEN_HEREDOC_DELIM);  // Adiciona como um token distinto
    *input += i;
}

void handle_quotes(char **input, t_token **tokens, char quote)
{
    char *value;
    int i = 1;

    while ((*input)[i] && (*input)[i] != quote)
        i++;
    if ((*input)[i] == quote)
    {
        value = ft_substr(*input, 1, i - 1);
        add_token(tokens, value, TOKEN_WORD);
        *input += i + 1;
    }
    else
        exit_with_error("Unclosed quote");
}

void handle_word(char **input, t_token **tokens)
{
    char *value;
    int i = 0;

    while ((*input)[i] && !is_space((*input)[i]) && !is_operator((*input)[i]) && !is_quote((*input)[i]))
        i++;
    value = ft_substr(*input, 0, i);
    add_token(tokens, value, TOKEN_WORD);
    *input += i;
}

char *read_heredoc_content(char *delimiter)
{
    char    *line;
    char    *content = NULL;
    char    *temp;

    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
            break;
        }
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        temp = content;
        content = ft_strjoin(content, line);
        free(temp);
        free(line);
        temp = content;
        content = ft_strjoin(content, "\n");
        free(temp);
    }
    return (content);
}




