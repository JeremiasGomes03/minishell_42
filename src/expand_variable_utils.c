/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:03:08 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/28 16:05:44 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*get_envp_value(t_shell *shell, char *var_name)
{
	char	**envp;
	int		var_len;

	envp = shell->envp;
	while (*envp)
	{
		var_len = ft_findchr(*envp, '=');
		if (var_len != -1 && ft_strncmp(var_name, *envp, var_len) == 0)
			return (ft_strdup(*envp + var_len + 1));
		envp++;
	}
	return (NULL);
}

static char	*extract_var_name_and_value(t_shell *shell, char *cmd, int *i)
{
	int		start;
	int		len;
	char	*var_name;
	char	*var_value;

	start = *i;
	if (cmd[*i] == '{')
		(*i)++;
	len = 0;
	while (cmd[*i + len] && (ft_isalnum(cmd[*i + len]) || cmd[*i + len] == '_'))
		len++;
	*i += len;
	if (cmd[*i] == '}')
		(*i)++;
	var_name = ft_substr(cmd, start, *i - start);
	if (!var_name)
		return (NULL);
	var_value = get_envp_value(shell, var_name);
	free(var_name);
	if (!var_value)
		var_value = ft_strdup("");
	return (var_value);
}

char	*get_envp(t_shell *shell, char *cmd)
{
	char	*var_value;
	int		i;

	i = 0;
	if (!cmd || !shell || !shell->envp)
		return (ft_strdup(""));
	if (*cmd == '?')
		return (ft_itoa(shell->exit_status));
	if (*cmd == '{' || ft_isalnum(*cmd))
	{
		var_value = extract_var_name_and_value(shell, cmd, &i);
		return (var_value);
	}
	return (ft_strdup(""));
}
