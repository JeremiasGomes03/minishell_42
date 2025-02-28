/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:18:05 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/27 21:59:26 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void exit_with_error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int validate_syntax(t_token *tokens)
{
    t_token *current = tokens;

    while (current)
    {
        // Verifica se há um pipe sem comandos antes ou depois
        if (current->type == TOKEN_PIPE)
        {
            if (!current->next || current->next->type == TOKEN_PIPE)
            {
                printf("Syntax error near unexpected token `|'\n");
                return (0);
            }
        }
        // Verifica se há um redirecionamento sem arquivo
        else if (is_redirection(current->type))
        {
            if (!current->next || current->next->type != TOKEN_WORD)
            {
                printf("Syntax error near unexpected token `%s'\n", current->value);
                return (0);
            }
        }
        current = current->next;
    }
    return (1); // Sintaxe válida
}

char *expand_variables(char *input)
{
    char    *result = NULL;
    char    *var_name;
    char    *var_value;
    int     i = 0;

    while (input[i])
    {
        if (input[i] == '$' && (ft_isalnum(input[i + 1]) || input[i + 1] == '_'))
        {
            i++;
            var_name = NULL;
            while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
            {
                var_name = ft_strjoin_char(var_name, input[i]);
                i++;
            }
            var_value = getenv(var_name); // Obtém o valor da variável de ambiente
            if (var_value)
                result = ft_strjoin_with_free(result, var_value, 1); // Adiciona o valor ao resultado
            free(var_name);
        }
        else
        {
            result = ft_strjoin_char(result, input[i]);
            i++;
        }
    }
    return (result);
}

void expand_command_vars(t_cmd_node *cmd)
{
    int i = 0;

    while (cmd->args && cmd->args[i])
    {
        cmd->args[i] = expand_variables(cmd->args[i]);
        i++;
    }
}