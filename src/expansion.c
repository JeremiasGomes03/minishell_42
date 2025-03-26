/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:27:11 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/25 23:52:38 by jeremias         ###   ########.fr       */
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
			else if (cmd[i] == '{')
			{
				int j = i + 1;
				while (cmd[j] && (ft_isalnum(cmd[j]) || cmd[j] == '_'))
					j++;
				if (cmd[j] == '}')
				{
					var_name = ft_substr(cmd, i + 1, j - i - 1);
					i = j + 1;
				}
				else
					var_name = NULL;
			}
			else
			{
				int j = i;
				while (cmd[j] && (ft_isalnum(cmd[j]) || cmd[j] == '_'))
					j++;
				var_name = ft_substr(cmd, i, j - i);
				i = j;
			}
			
			if (var_name)
			{
				var_value = get_envp(shell, var_name);
				result = ft_strjoin_with_free(result, var_value, 1);
				free(var_value);
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

char    *get_envp(t_shell *shell, char *cmd)
{
	char    *var_name;
	char    *var_value;
	int     len;
	int     var_len;
	char    **envp = shell->envp;

	if (!cmd || !shell || !shell->envp)
		return (ft_strdup(""));
	if (*cmd == '?')
		return (ft_itoa(shell->exit_status));

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
			continue;
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
