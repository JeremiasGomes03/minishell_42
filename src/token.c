/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:56:03 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/27 22:09:55 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token *tokenize_input(char *input)
{
    t_token *tokens = NULL;

    while (*input)
    {
        if (is_space(*input))
            input++;
        else if (is_operator(*input))
            handle_operator(&input, &tokens);
        else if (is_quote(*input))
            handle_quotes(&input, &tokens, *input);
        else
            handle_word(&input, &tokens);
    }

    if (tokens)
        add_token(&tokens, NULL, TOKEN_EOF);

    return tokens;
}

void add_token(t_token **tokens, char *value, t_token_type type)
{
    t_token *new = malloc(sizeof(t_token));
    t_token *last;

    if (!new)
        exit_with_error("Failed to allocate memory");
    new->value = value ? ft_strdup(value) : NULL;
    new->type = type;
    new->next = NULL;

    if (!*tokens)
    {
        *tokens = new;
        return;
    }

    last = *tokens;
    while (last->next)
        last = last->next;
    last->next = new;
}

void handle_env_vars(char **input, t_token **tokens)
{
    char    *var_name;
    char    *var_value;
    int     i = 0;

    if ((*input)[0] == '$')
    {
        (*input)++; // Avança o ponteiro após o '$'
        while ((*input)[i] && (ft_isalnum((*input)[i]) || (*input)[i] == '_'))
            i++;
        var_name = ft_substr(*input, 0, i); // Captura o nome da variável
        var_value = getenv(var_name); // Obtém o valor da variável de ambiente
        if (var_value)
            add_token(tokens, ft_strdup(var_value), TOKEN_WORD); // Adiciona o valor como token
        free(var_name);
        *input += i; // Avança o ponteiro de input
    }
}
char *ft_strjoin_char(char *s1, char c)
{
    char    *result;
    int     len;
    int     i;

    if (!s1)
        return (NULL);

    // Calcula o tamanho da string s1
    len = ft_strlen(s1);

    // Aloca memória para a nova string (s1 + c + '\0')
    result = malloc(sizeof(char) * (len + 2));
    if (!result)
        return (NULL);

    // Copia s1 para a nova string
    i = 0;
    while (s1[i])
    {
        result[i] = s1[i];
        i++;
    }

    // Adiciona o caractere c ao final da nova string
    result[i] = c;
    result[i + 1] = '\0';

    // Libera a memória da string original (s1)
    free(s1);

    return (result);
}