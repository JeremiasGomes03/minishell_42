/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:45:47 by lamachad          #+#    #+#             */
/*   Updated: 2025/03/25 21:37:04 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	close_fds(t_cmd_node *cmd)
{
	if (cmd->in_fd != STDIN_FILENO)
	{
		close(cmd->in_fd);
		cmd->in_fd = STDIN_FILENO;
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		close(cmd->out_fd);
		cmd->out_fd = STDOUT_FILENO;
	}
}

static void	redirect_io(t_cmd_node *cmd)
{
	if (cmd->in_fd != STDIN_FILENO)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->in_fd);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->out_fd);
	}
}

static void	execute_builtin_command(t_cmd_node *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		builtin_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		builtin_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		builtin_pwd(cmd);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		builtin_export(shell, cmd->args[1]);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		builtin_unset(shell, cmd->args[1]);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd);
}

int	is_builtin(t_cmd_node *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (!ft_strcmp(cmd->args[0], "echo") || !ft_strcmp(cmd->args[0], "cd")
		|| !ft_strcmp(cmd->args[0], "pwd") || !ft_strcmp(cmd->args[0], "export")
		|| !ft_strcmp(cmd->args[0], "unset") || !ft_strcmp(cmd->args[0], "env")
		|| !ft_strcmp(cmd->args[0], "exit"))
		return (1);
	return (0);
}

void	execute_builtin(t_cmd_node *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		shell->exit_status = 1;
		return ;
	}
	else if (pid == 0)
	{
		setup_child_signals();
		redirect_io(cmd);
		execute_builtin_command(cmd, shell);
		exit(shell->exit_status);
	}
	else
	{
		close_fds(cmd);
		waitpid(pid, &status, 0);
		shell->exit_status = WEXITSTATUS(status);
	}
}
