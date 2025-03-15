/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavinia <lavinia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:55:24 by lavinia           #+#    #+#             */
/*   Updated: 2025/03/14 18:30:39 by lavinia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	builtin_exit(t_cmd_node *cmd)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->args[1])
	{
		exit_code = ft_atoi(cmd->args[1]);
		if (!ft_isnumeric(cmd->args[1]))
		{
			printf("exit: %s: numeric argument required\n", cmd->args[1]);
			exit_code = 255;
		}
	}
	printf("exit\n");
	exit(exit_code);
}

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

