/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavinia <lavinia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:45:47 by lamachad          #+#    #+#             */
/*   Updated: 2025/03/15 16:19:44 by lavinia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(t_cmd_node *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (!ft_strcmp(cmd->args[0], "echo") || !ft_strcmp(cmd->args[0], "cd") ||
		!ft_strcmp(cmd->args[0], "pwd") || !ft_strcmp(cmd->args[0], "export") ||
		!ft_strcmp(cmd->args[0], "unset") || !ft_strcmp(cmd->args[0], "env") ||
		!ft_strcmp(cmd->args[0], "exit"))
		return (1);
	return (0);
}

void	execute_builtin(t_cmd_node *cmd, t_shell *shell)
{
	if (!cmd || !cmd->args || !cmd->args[0] || !shell)
		return ;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		builtin_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		builtin_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
	{
		if (cmd->args[1])
			builtin_export(shell, cmd->args[1]);
	}
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
	{
		if (cmd->args[1])
			builtin_unset(shell, cmd->args[1]);
	}
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		builtin_pwd(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		builtin_env(shell);
}
