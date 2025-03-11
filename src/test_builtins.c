/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamachad <lamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:45:47 by lamachad          #+#    #+#             */
/*   Updated: 2025/03/11 09:02:24 by lamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(t_cmd_node *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (1);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (1);
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (1);
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (1);
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (1);
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_cmd_node *cmd)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		builtin_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		builtin_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		builtin_export(cmd);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		builtin_unset(cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		builtin_pwd(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		builtin_env(cmd);
}


int	main(void)
{
	t_cmd_node cmd;

	ft_bzero(&cmd, sizeof(t_cmd_node));
	char *echo_args[] = {"echo", "Hello,", "world!", NULL};
	cmd.args = echo_args;
	builtin_echo(&cmd);

	// Teste do pwd
	ft_bzero(&cmd, sizeof(t_cmd_node)); // Limpa antes de reutilizar
	char *pwd_args[] = {"pwd", NULL};
	cmd.args = pwd_args;
	builtin_pwd(&cmd);

	// Teste do cd
	ft_bzero(&cmd, sizeof(t_cmd_node));
	char *cd_args[] = {"cd", "..", NULL};
	cmd.args = cd_args;
	builtin_cd(&cmd);

	// Verifica se `cd` funcionou
	builtin_pwd(&cmd);

	// Teste do export
	ft_bzero(&cmd, sizeof(t_cmd_node));
	char *export_args[] = {"export", "TEST_VAR=42", NULL};
	cmd.args = export_args;
	builtin_export(&cmd);

	// Verifica se `export` funcionou
	builtin_env(&cmd);

	// Teste do unset
	ft_bzero(&cmd, sizeof(t_cmd_node));
	char *unset_args[] = {"unset", "TEST_VAR", NULL};
	cmd.args = unset_args;
	builtin_unset(&cmd);

	// Verifica se `unset` funcionou
	builtin_env(&cmd);

	// Teste do env
	ft_bzero(&cmd, sizeof(t_cmd_node));
	builtin_env(&cmd);

	// Teste do exit (remova o comentário para testar)
	// char *exit_args[] = {"exit", "2", NULL};
	// cmd.args = exit_args;
	// builtin_exit(&cmd);

	return (0);
}

