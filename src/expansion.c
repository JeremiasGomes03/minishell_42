/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:27:11 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/03 23:46:12 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *expand_variables(char *cmd, t_shell *shell)
{
    char *result = NULL;
    char *var_name;
    char *var_value;
    int i = 0;
    int quotes = 0;

    if (!cmd)
        return (NULL);
    while (cmd[i])
    {
        quotes = check_quotes(cmd[i], quotes);
        if (cmd[i] == '$' && quotes != 2)
        {
            i++;
            var_name = NULL;
            while (cmd[i] && (ft_isalnum(cmd[i]) || cmd[i] == '_'))
            {
                var_name = ft_strjoin_char(var_name, cmd[i]);
                i++;
            }
            if (var_name)
            {
                var_value = my_getenv(var_name, shell->envp);
                if (!var_value)
                {
                    printf("Erro ao expandir a variável: $%s\n", var_name);
                    free(var_name);
                    return (NULL);
                }
                result = ft_strjoin_with_free(result, var_value, 1);
                free(var_name);
            }
        }
        else
        {
            result = ft_strjoin_char(result, cmd[i]);
            if (!result)
                return (NULL);
            i++;
        }
    }
    return (result);
}


char *my_getenv(const char *var_name, char **envp)
{
    int i = 0;
    size_t len = ft_strlen(var_name);

    while (envp[i])
    {
        printf("Verificando %s em envp[%d]: %s\n", var_name, i, envp[i]);
        if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
        {
            printf("Encontrado %s=%s\n", var_name, envp[i] + len + 1);
            return (envp[i] + len + 1);
        }
        i++;
    }
    printf("Variável %s não encontrada\n", var_name);
    return (NULL);
}



int check_quotes(char c, int quotes)
{
    if (c == '\"' && quotes != 2)
        return (quotes == 1 ? 0 : 1);
    if (c == '\'' && quotes != 1)
        return (quotes == 2 ? 0 : 2);
    return (quotes);
}

void expander(t_token **head, t_shell *shell)
{
    t_token *token = *head;
    char    *temp;

    while (token)
    {
        if (ft_strchr(token->value, '$')) // Verifica se há variáveis para expandir
        {
            temp = token->value;
            token->value = expand_variables(token->value, shell); // Passando shell para expandir variáveis
            if (!token->value) // Se a expansão falhar, mantém o valor original
                token->value = temp;
            else
                free(temp); // Libera a memória do valor original
        }
        token = token->next;
    }
}
char *ft_strjoin_with_free(char *s1, char *s2, int free_s1)
{
    char *result = ft_strjoin(s1, s2);
    if (free_s1 && s1)
        free(s1);
    return (result);
}

char *ft_strjoin_char(char *s1, char c)
{
    char    *result;
    int     len;
    int     i;

    if (!s1)
        return (NULL);
    len = ft_strlen(s1);
    result = malloc(sizeof(char) * (len + 2));
    if (!result)
        return (NULL);
    i = 0;
    while (s1[i])
    {
        result[i] = s1[i];
        i++;
    }
    result[i] = c;
    result[i + 1] = '\0';
    free(s1);
    return (result);
}

