/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavinia <lavinia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:45:47 by lamachad          #+#    #+#             */
/*   Updated: 2025/03/31 18:39:22 by lavinia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(t_cmd_node *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	return (!ft_strcmp(cmd->args[0], "echo")
		|| !ft_strcmp(cmd->args[0], "cd")
		|| !ft_strcmp(cmd->args[0], "pwd")
		|| !ft_strcmp(cmd->args[0], "export")
		|| !ft_strcmp(cmd->args[0], "unset")
		|| !ft_strcmp(cmd->args[0], "env")
		|| !ft_strcmp(cmd->args[0], "exit"));
}

int	is_env_modifying_builtin(t_cmd_node *cmd)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	return (
		ft_strcmp(cmd->args[0], "export") == 0
		|| ft_strcmp(cmd->args[0], "unset") == 0
		|| ft_strcmp(cmd->args[0], "cd") == 0
		|| ft_strcmp(cmd->args[0], "exit") == 0
	);
}

void	execute_builtin(t_cmd_node *cmd, t_shell *shell)
{
	int	i;

	i = 0;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		builtin_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		builtin_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
	{
		while (cmd->args[i])
		{
			builtin_export(shell, cmd->args[i]);
			i++;
		}
	}
	else if (ft_strcmp(cmd->args[0], "unset") == 0 && cmd->args[1])
		builtin_unset(shell, cmd->args[1]);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		builtin_pwd(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd);  // Aqui a função `builtin_exit` é chamada corretamente
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		builtin_env(shell);
}


int ft_exit(char **args)
{
    int exit_code = 0;

    // Verifica se o usuário forneceu um argumento
    if (args[1] != NULL)
    {
        char *endptr;

        // Tenta converter o argumento para um número inteiro
        exit_code = strtol(args[1], &endptr, 10);

        // Verifica se a conversão foi bem-sucedida
        if (*endptr != '\0')  // Se endptr não aponta para o final da string, não é um número válido
        {
            // Mensagem de erro usando write (sem fprintf)
            write(2, "minishell: exit: numeric argument required\n", 41);
            return 255; // Código de erro por argumento inválido
        }
    }

    // Exibe a mensagem de saída
    write(1, "exit\n", 5);
    exit(exit_code); // Finaliza o programa com o código de saída
}

