/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavinia <lavinia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/15 16:18:36 by lavinia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	builtin_echo(t_cmd_node *cmd)
{
	int	i;
	int	newline;

	if (!cmd || !cmd->args)
		return ;
	i = 1;
	newline = 1;
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], 1);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
}

void	builtin_cd(t_cmd_node *cmd)
{
	char	*dir;

	if (!cmd->args[1])
		dir = getenv("HOME");
	else
		dir = cmd->args[1];
	if (chdir(dir) == -1)
		perror("cd");
}

void	builtin_export(t_shell *shell, char *var)
{
	char	**new_envp;
	int		i;

	if (!shell || !var)
		return ;
	i = 0;
	while (shell->envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
		return ;
	i = 0;
	while (shell->envp[i])
	{
		new_envp[i] = shell->envp[i]; // Mantém as variáveis antigas
		i++;
	}
	new_envp[i] = strdup(var); // Adiciona a nova variável
	new_envp[i + 1] = NULL;

	free(shell->envp);
	shell->envp = new_envp;
}


void	builtin_unset(t_shell *shell, char *var)
{
	char	**new_envp;
	int		i;
	int		j;
	int		len;

	if (!shell || !var)
		return ;
	len = strlen(var);
	i = 0;
	while (shell->envp[i])
	{
		if (strncmp(shell->envp[i], var, len) == 0 && shell->envp[i][len] == '=')
			break ;
		i++;
	}
	if (!shell->envp[i])
		return ;
	new_envp = malloc(sizeof(char *) * i);
	if (!new_envp)
		return ;
	j = 0;
	i = 0;
	while (shell->envp[i])
	{
		if (strncmp(shell->envp[i], var, len) == 0 && shell->envp[i][len] == '=')
			free(shell->envp[i]);
		else
		{
			new_envp[j] = shell->envp[i];
			j++;
		}
		i++;
	}
	new_envp[j] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
}


void	builtin_pwd(t_cmd_node *cmd)
{
	char	*cwd;

	(void)cmd;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_putendl_fd(cwd, 1);
		free(cwd);
	}
	else
		perror("pwd");
}