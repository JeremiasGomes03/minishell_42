/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/26 22:30:05 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void handle_env_vars(char **input, t_token **tokens)
{
    char    *var_name;
    char    *var_value;
    int     i = 0;

    if ((*input)[0] == '$')
    {
        (*input)++;
        while ((*input)[i] && (ft_isalnum((*input)[i]) || (*input)[i] == '_'))
            i++;
        var_name = ft_substr(*input, 0, i);
        var_value = getenv(var_name);
        if (var_value)
            add_token(tokens, ft_strdup(var_value), TOKEN_WORD);
        free(var_name);
        *input += i;
    }
}

void handle_escaped_chars(char **input, t_token **tokens)
{
    char    *value;

    if ((*input)[0] == '\\')
    {
        (*input)++;
        value = malloc(2);
        if (!value)
            exit_with_error("Failed to allocate memory");
        value[0] = (*input)[0];
        value[1] = '\0';
        add_token(tokens, value, TOKEN_WORD);
        (*input)++;
    }
}

void handle_comments(char **input)
{
    if ((*input)[0] == '#')
    {
        while ((*input)[0] && (*input)[0] != '\n')
            (*input)++;
    }
}
