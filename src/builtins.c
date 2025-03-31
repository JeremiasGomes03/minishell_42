/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavinia <lavinia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/31 18:39:01 by lavinia          ###   ########.fr       */
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
		char *endptr;

		// Tenta converter o argumento para um número inteiro
		exit_code = strtol(cmd->args[1], &endptr, 10);

		// Verifica se a conversão foi bem-sucedida
		if (*endptr != '\0')  // Se endptr não aponta para o final da string, não é um número válido
		{
			// Mensagem de erro usando write (sem fprintf)
			write(2, "minishell: exit: numeric argument required\n", 41);
			exit_code = 255;
		}
	}
	// Exibe a mensagem de saída usando write
	write(1, "exit\n", 5);
	exit(exit_code); // Finaliza o programa com o código de saída
}
