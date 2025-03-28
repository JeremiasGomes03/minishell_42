/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:30:03 by lavinia           #+#    #+#             */
/*   Updated: 2025/03/28 14:32:54 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void export_list(t_shell *shell)
{
	int		i;
	char	*entry;
	char	*equal_sign;
	int		key_len;

	if (!shell || !shell->envp)
		return;

	i = 0;
	while (shell->envp[i])
	{
		entry = shell->envp[i];
		equal_sign = ft_strchr(entry, '=');
		if (equal_sign)
		{
			key_len = equal_sign - entry;
			write(1, "declare -x ", 11);
			write(1, entry, key_len);
			write(1, "=\"", 2);
			write(1, equal_sign + 1, ft_strlen(equal_sign + 1));
			write(1, "\"\n", 2);
		}
		else
		{
			write(1, "declare -x ", 11);
			write(1, entry, ft_strlen(entry));
			write(1, "\n", 1);
		}
		i++;
	}
}

void export_add_or_replace(t_shell *shell, char *var)
{
	int		i;
	int		j;
	char	*equals;
	char	**new_envp;

	if (!shell || !var)
		return;
	equals = ft_strchr(var, '=');
	if (!equals || equals == var)
		return;
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var, equals - var) == 0 &&
			shell->envp[i][equals - var] == '=')
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strdup(var);
			return;
		}
		i++;
	}
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
		return;
	j = 0;
	while (j < i)
	{
		new_envp[j] = ft_strdup(shell->envp[j]);
		j++;
	}
	new_envp[j++] = ft_strdup(var);
	new_envp[j] = NULL;
	j = 0;
	while (shell->envp[j])
	{
		free(shell->envp[j]);
		j++;
	}
	free(shell->envp);
	shell->envp = new_envp;
}

char *sanitize_export_arg(char *arg)
{
    int len;

    if (!arg)
        return NULL;
    len = ft_strlen(arg);
    if (arg[0] == '"' && arg[len - 1] == '"')
    {
        return ft_substr(arg, 1, len - 2);
    }
    if (arg[0] == '\'' && arg[len - 1] == '\'')
    {
        return ft_substr(arg, 1, len - 2);
    }
    return ft_strdup(arg);
}
