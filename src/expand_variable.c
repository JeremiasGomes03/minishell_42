/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:27:11 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/31 00:15:20 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*extract_var_name(char *cmd, int *i)
{
	int	start;

	start = *i;
	if (cmd[*i] == '{')
		(*i)++;
	while (cmd[*i] && (ft_isalnum(cmd[*i]) || cmd[*i] == '_'))
		(*i)++;
	if (cmd[*i] == '}')
		(*i)++;
	return (ft_substr(cmd, start, *i - start));
}

static char	*process_variable(char *cmd, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;

	if (cmd[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	var_name = extract_var_name(cmd, i);
	var_value = get_envp(shell, var_name);
	free(var_name);
	if (!var_value)
		var_value = ft_strdup("");
	return (var_value);
}

static int	handle_expansion(char *cmd, int *i, char **result, t_shell *shell)
{
	char	*var_value;

	var_value = process_variable(cmd, i, shell);
	*result = ft_strjoin_with_free(*result, var_value, 1);
	free(var_value);
	return (0);
}

char	*expand_variables_loop(char *cmd, t_quote_type quote_type,
	t_shell *shell, char *result)
{
	int	i;
	int	in_quotes;

	i = 0;
	in_quotes = 0;
	while (cmd[i])
	{
		if (quote_type == NO_QUOTES)
			in_quotes = check_quotes(cmd[i], in_quotes);
		if (cmd[i] == '$' && (quote_type == DOUBLE_QUOTES || !in_quotes))
		{
			if (cmd[i + 1] && (cmd[i + 1] == '?' || cmd[i + 1] == '{'
					|| ft_isalnum(cmd[i + 1]) || cmd[i + 1] == '_'))
			{
				i++;
				handle_expansion(cmd, &i, &result, shell);
			}
			else
				result = ft_strjoin_char(result, '$');
		}
		else
			result = ft_strjoin_char(result, cmd[i]);
		i++;
	}
	return (result);
}

char	*expand_variables(char *cmd, t_quote_type quote_type, t_shell *shell)
{
	char	*result;

	if (!cmd)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	if (quote_type == SINGLE_QUOTES)
		return (free(result), ft_strdup(cmd));
	return (expand_variables_loop(cmd, quote_type, shell, result));
}
