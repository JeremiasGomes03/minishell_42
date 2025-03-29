/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:27:11 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/29 02:12:46 by jeremias         ###   ########.fr       */
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

static int	handle_normal_char(char *cmd, int i, char **result)
{
	*result = ft_strjoin_char(*result, cmd[i]);
	if (!*result)
		return (1);
	return (0);
}

char	*expand_variables(char *cmd, t_shell *shell)
{
	char	*result;
	int		i;
	int		quotes;

	i = 0;
	quotes = 0;
	result = ft_strdup("");
	if (!result || !cmd)
		return (NULL);
	while (cmd[i])
	{
		quotes = check_quotes(cmd[i], quotes);
		if (cmd[i] == '$' && quotes != 2)
		{
			i++;
			if (handle_expansion(cmd, &i, &result, shell))
				return (NULL);
		}
		else
		{
			if (handle_normal_char(cmd, i++, &result))
				return (NULL);
		}
	}
	return (result);
}
