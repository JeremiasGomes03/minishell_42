/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:22:33 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/28 17:09:08 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_command_args(t_cmd_node *cmd, t_shell *shell)
{
	int		i;
	char	*expanded_arg;
	char	*temp;
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
				return (printf("Erro ao expandir variáveis no argumento: %s\n",
						cmd->args[i]));
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

void	execute_command(t_cmd_node *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (fprintf(stderr, "minishell: command not found\n"),
			shell->exit_status = 127);
	if (is_builtin(cmd))
		return (execute_builtin(cmd, shell));
	if (cmd->in_fd == -1)
		return (fprintf(stderr, "minishell: No such file or directory\n"),
			shell->exit_status = 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"));
	if (pid == 0)
	{
		setup_child_signals();
		execute_child(cmd);
	}
	waitpid(pid, &status, 0);
	shell->exit_status = WEXITSTATUS(status);
}
