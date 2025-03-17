/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavinia <lavinia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:45:47 by lamachad          #+#    #+#             */
/*   Updated: 2025/03/16 00:23:22 by lavinia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(t_cmd_node *cmd)
{
	const char	*builtins[7];
	int			i;

	if (!cmd || !cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		return (0);
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	i = 0;
	while (i < 7)
	{
		if (!ft_strcmp(cmd->args[0], builtins[i]))
			return (1);
		i++;
	}
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
		builtin_export(shell, cmd->args[1]);
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

