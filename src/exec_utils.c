/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavinia <lavinia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:22:33 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/28 20:22:01 by lavinia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_command_args(t_cmd_node *cmd, t_shell *shell)
{
	int		i;
	char	*expanded_arg;
	int		quotes;

	i = 0;
	quotes = 0;
	while (cmd->args && cmd->args[i])
	{
		quotes = check_quotes(cmd->args[i][0], quotes);
		if (cmd->args[i][0] == '\1')
			cmd->args[i] = ft_strdup(cmd->args[i] + 1);
		else if (ft_strchr(cmd->args[i], '$') && quotes != 2)
		{
			expanded_arg = expand_variables(cmd->args[i], shell);
			if (!expanded_arg)
			{
				printf("Err expandir variáveis: %s\n", cmd->args[i]);
				return ;
			}
			cmd->args[i] = expanded_arg;
		}
		if (!cmd->args[i])
			return (perror("ft_strdup"), exit(EXIT_FAILURE));
		i++;
	}
}

void	execute_child(t_cmd_node *cmd)
{
	setup_child_signals();
	if (cmd->in_fd != STDIN_FILENO)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->in_fd);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->out_fd);
	}
	execvp(cmd->args[0], cmd->args);
	fprintf(stderr, "minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(EXIT_FAILURE);
}

// static void	execute_external_command(t_cmd_node *cmd, t_shell *shell)
// {
// 	pid_t	pid;
// 	int		status;

// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		return ;
// 	}
// 	if (pid == 0)
// 	{
// 		setup_child_signals();
// 		execute_child(cmd);
// 	}
// 	waitpid(pid, &status, 0);
// 	shell->exit_status = WEXITSTATUS(status);
// }

// void	execute_command(t_cmd_node *cmd, t_shell *shell)
// {
// 	if (!cmd || !cmd->args || !cmd->args[0])
// 	{
// 		fprintf(stderr, "minishell: command not found\n");
// 		shell->exit_status = 127;
// 		return ;
// 	}
// 	if (is_builtin(cmd))
// 	{
// 		execute_builtin(cmd, shell);
// 		return ;
// 	}
// 	if (cmd->in_fd == -1)
// 	{
// 		fprintf(stderr, "minishell: No such file or directory\n");
// 		shell->exit_status = 1;
// 		return ;
// 	}
// 	execute_external_command(cmd, shell);
// }
