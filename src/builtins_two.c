/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:55:24 by lavinia           #+#    #+#             */
/*   Updated: 2025/03/27 20:53:52 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void	builtin_env(t_shell *shell)
{
	int	i;

	if (!shell || !shell->envp)
		return ;

	i = 0;
	while (shell->envp[i])
	{
		printf("%s\n", shell->envp[i]);
		i++;
	}
}

static void	free_envp_array(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static int	count_env_vars(char **envp, char *var)
{
	int	count;
	int	len;

	count = 0;
	len = ft_strlen(var);
	while (envp[count])
	{
		if (ft_strncmp(envp[count], var, len) == 0 && envp[count][len] == '=')
			return (count);
		count++;
	}
	return (-1);
}

static char	**create_new_envp(char **old_envp, int skip_index)
{
	char	**new_envp;
	int		count;
	int		i;
	int		j;

	count = 0;
	while (old_envp[count])
		count++;
	if (!(new_envp = malloc(sizeof(char *) * (count))))
		return (NULL);
	i = -1;
	j = 0;
	while (old_envp[++i])
	{
		if (i != skip_index)
		{
			if (!(new_envp[j] = strdup(old_envp[i])))
			{
				free_envp_array(new_envp);
				return (NULL);
			}
			j++;
		}
	}
	new_envp[j] = NULL;
	return (new_envp);
}

void	builtin_unset(t_shell *shell, char *var)
{
	int		index;
	char	**new_envp;

	if (!shell || !var)
		return ;
	index = count_env_vars(shell->envp, var);
	if (index == -1)
		return ;
	new_envp = create_new_envp(shell->envp, index);
	if (!new_envp)
		return ;
	free_envp_array(shell->envp);
	shell->envp = new_envp;
}
