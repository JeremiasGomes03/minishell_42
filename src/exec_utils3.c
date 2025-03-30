/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:32:45 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/29 19:47:51 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	close_fds(int fd1, int fd2)
{
	if (fd1 != -1)
		close(fd1);
	if (fd2 != -1)
		close(fd2);
}

static void	close_command_fds(t_cmd_node *cmd)
{
	if (cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
}

int	handle_parent_builtin(t_cmd_node *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
		return (perror("minishell: dup"), -1);
	if (cmd->in_fd != STDIN_FILENO && dup2(cmd->in_fd, STDIN_FILENO) == -1)
		return (close_fds(saved_stdin, saved_stdout), -1);
	if (cmd->out_fd != STDOUT_FILENO && dup2(cmd->out_fd, STDOUT_FILENO) == -1)
		return (close_fds(saved_stdin, saved_stdout), -1);
	execute_builtin(cmd, shell);
	if (dup2(saved_stdin, STDIN_FILENO) == -1
		|| dup2(saved_stdout, STDOUT_FILENO) == -1)
		return (close_fds(saved_stdin, saved_stdout), -1);
	close_fds(saved_stdin, saved_stdout);
	close_command_fds(cmd);
	return (0);
}

int	has_non_heredoc_redirs(t_cmd_node *cmd)
{
	t_redir	*current_redir;

	if (!cmd || !cmd->redirections)
		return (0);
	current_redir = cmd->redirections;
	while (current_redir)
	{
		if (current_redir->type != REDIR_HEREDOC)
			return (1);
		current_redir = current_redir->next;
	}
	return (0);
}
