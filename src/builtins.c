/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/29 01:36:06 by jeremias         ###   ########.fr       */
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
	char	*cleaned_var;

	if (!shell)
		return ;
	if (!var)
	{
		export_list(shell);
	}
	else
	{
		cleaned_var = sanitize_export_arg(var);
		export_add_or_replace(shell, cleaned_var);
		free(cleaned_var);
	}
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
