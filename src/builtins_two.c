/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavinia <lavinia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:55:24 by lavinia           #+#    #+#             */
/*   Updated: 2025/03/08 21:09:05 by lavinia          ###   ########.fr       */
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

void	builtin_env(t_cmd_node *cmd)
{
	int	i;

	(void)cmd;
	i = 0;
	while (environ[i])
	{
		ft_putendl_fd(environ[i], 1);
		i++;
	}
}

