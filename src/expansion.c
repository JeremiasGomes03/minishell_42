/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:27:11 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/20 16:28:06 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char    *expand_variables(char *cmd, t_shell *shell)
{
    char    *result;
    char    *var_name;
    char    *var_value;
    int     i;
    int     quotes;

    i = 0;
    quotes = 0;
    if (!cmd)
        return (NULL);
    result = ft_strdup("");
    if (!result)
        return (NULL);
    while (cmd[i])
    {
        quotes = check_quotes(cmd[i], quotes);
        if (cmd[i] == '$' && quotes != 2)
        {
            i++;
            if (cmd[i] == '?')
            {
                var_value = ft_itoa(shell->exit_status);
                result = ft_strjoin_with_free(result, var_value, 1);
                free(var_value);
                i++;
            }
            else
            {
                var_name = NULL;
                while (cmd[i] && (ft_isalnum(cmd[i]) || cmd[i] == '_'))
                {
                    var_name = ft_strjoin_char(var_name, cmd[i]);
                    i++;
                }
                if (var_name)
                {
                    var_value = get_envp(shell, var_name);
                    if (!var_value)
                    {
                        free(var_name);
                        return (NULL);
                    }
                    result = ft_strjoin_with_free(result, var_value, 1);
                    free(var_value);
                    free(var_name);
                }
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


static int	ft_findchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}


char	*get_envp(t_shell *shell, char *cmd)
{
	char	*var_name;
	char	*var_value;
	int		len;
	int		var_len;
	char	**envp = shell->envp;

	if (!cmd || !shell || !shell->envp)
		return (NULL);
	if (*cmd == '?')
		return (ft_itoa(shell->exit_status));
	if (*cmd == '{')
		cmd++;
	len = 0;
	while (cmd[len] && (ft_isalnum(cmd[len]) || cmd[len] == '_'))
		len++;
	if (len == 0)
		return (ft_strdup(""));
	var_name = ft_substr(cmd, 0, len);
	if (!var_name)
		return (NULL);
	while (*envp)
	{
		var_len = ft_findchr(*envp, '=');
		if (var_len == -1)
		{
			envp++;
			continue ;
		}
		if (ft_strncmp(var_name, *envp, var_len) == 0)
		{
			var_value = ft_strdup(*envp + var_len + 1);
			free(var_name);
			return (var_value);
		}
		envp++;
	}
    free(var_name);
    return (ft_strdup("")); 
}

int check_quotes(char c, int quotes)
{
    if (c == '\"' && quotes != 2)
        return (quotes == 1 ? 0 : 1);
    if (c == '\'' && quotes != 1)
        return (quotes == 2 ? 0 : 2);
    return quotes;
}

void expander(t_token **head, t_shell *shell)
{
    t_token *current = *head;
    while (current)
    {
        if (current->quote_type != SINGLE_QUOTES && ft_strchr(current->value, '$'))
        {
            char *temp = current->value;
            current->value = expand_variables(current->value, shell);
            if (!current->value)
                current->value = temp;
            else
                free(temp);
        }
        current = current->next;
    }
}


char	*ft_strjoin_with_free(char *s1, char *s2, int free_s1)
{
	char *result = ft_strjoin(s1, s2);
	if (free_s1 && s1)
		free(s1);
	return (result);
}

char *ft_strjoin_char(char *s1, char c)
{
    char *result;
    size_t len, i;

    if (!s1)
    {
        result = malloc(2);
        if (!result)
            return NULL;
        result[0] = c;
        result[1] = '\0';
        return result;
    }
    len = ft_strlen(s1);
    result = malloc(len + 2);
    if (!result)
    {
        free(s1);
        return NULL;
    }
    for (i = 0; i < len; i++)
        result[i] = s1[i];
    result[len] = c;
    result[len + 1] = '\0';
    free(s1);
    return result;
}



