/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/01 01:02:22 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

int	builtin_echo(t_cmd_node *cmd)
{
	int	i;
	int	newline;

	if (!cmd || !cmd->args)
		return (0);
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
	return (0);
}

int	builtin_cd(t_cmd_node *cmd)
{
	char	*dir;

	if (!cmd->args[1])
		dir = getenv("HOME");
	else
		dir = cmd->args[1];
	if (chdir(dir) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	builtin_export(t_shell *shell, char *var)
{
	char	*cleaned_var;

	if (!shell)
		return (0);
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
	return (0);
}

int	builtin_pwd(t_cmd_node *cmd)
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
	return (0);
}

int	builtin_exit(t_cmd_node *cmd)
{
	int		exit_code;
	char	*endptr;

	exit_code = 0;
	if (cmd->args[1])
	{
		exit_code = strtol(cmd->args[1], &endptr, 10);
		if (*endptr != '\0' || errno == ERANGE)
		{
			write(2, "minishell: exit: numeric argument required\n", 45);
			return (255);
		}
		if (cmd->args[2])
		{
			write(2, "minishell: exit: too many arguments\n", 36);
			return (1);
		}
	}
	write(1, "exit\n", 5);
	exit((unsigned char)exit_code);
}
