/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavinia <lavinia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 21:19:14 by lavinia           #+#    #+#             */
/*   Updated: 2025/03/08 21:41:52 by lavinia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0); // Retorna 0 para falso
    if (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd") ||
        !strcmp(cmd, "export") || !strcmp(cmd, "unset") || !strcmp(cmd, "env") ||
        !strcmp(cmd, "exit"))
        return (1); // Retorna 1 para verdadeiro
    return (0);
}

void	execute_builtin(t_cmd_node *cmd)
{
	if (!cmd || !cmd->args[0])
		return;
	if (!strcmp(cmd->args[0], "echo"))
		builtin_echo(cmd);
	else if (!strcmp(cmd->args[0], "cd"))
		builtin_cd(cmd);
	else if (!strcmp(cmd->args[0], "pwd"))
		builtin_pwd();
	else if (!strcmp(cmd->args[0], "export"))
		builtin_export(cmd);
	else if (!strcmp(cmd->args[0], "unset"))
		builtin_unset(cmd);
	else if (!strcmp(cmd->args[0], "env"))
		builtin_env();
	else if (!strcmp(cmd->args[0], "exit"))
		builtin_exit(cmd);
}


void	prompt_loop(void)
{
	char		*line;
	t_cmd_node	cmd;

	while (1)
	{
		printf("minishell$ ");
		line = NULL;
		size_t len = 0;
		getline(&line, &len, stdin);
		if (!line)
			break ;
		
		// Remover o '\n' do final da entrada do usuário
		line[strcspn(line, "\n")] = 0;

		// Simular parsing básico (dividir a linha em argumentos)
		cmd.args = malloc(sizeof(char *) * 3);
		cmd.args[0] = strtok(line, " ");
		cmd.args[1] = strtok(NULL, " ");
		cmd.args[2] = NULL;

		// Executar builtins se for um deles
		if (is_builtin(cmd.args[0]))
			execute_builtin(&cmd);
		else
			printf("Comando não encontrado: %s\n", cmd.args[0]);

		free(cmd.args);
		free(line);
	}
}
