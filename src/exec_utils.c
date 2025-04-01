/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:22:33 by jeremias          #+#    #+#             */
/*   Updated: 2025/04/01 01:35:48 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_command_args(t_cmd_node *cmd, t_shell *shell)
{
	int	i;

	(void)shell;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		if (cmd->args[i][0] == '\1')
			cmd->args[i] = ft_strdup(cmd->args[i] + 1);
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

static char	*join_path(const char *dir, const char *cmd)
{
	char	*path;
	char	*full_path;

	path = ft_strjoin(dir, "/");
	if (!path)
		return (NULL);
	full_path = ft_strjoin(path, cmd);
	free(path);
	return (full_path);
}

char	*get_env_value(char **envp, const char *var)
{
	int		i;
	int		var_len;

	if (!envp || !var)
		return (NULL);
	var_len = ft_strlen(var);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, var_len) == 0 && envp[i][var_len] == '=')
			return (envp[i] + var_len + 1);
		i++;
	}
	return (NULL);
}

char	*get_absolute_path(const char *cmd, t_shell *shell)
{
	int		i;
	char	*path;
	char	**paths;

	if (!cmd || !*cmd)
		return (NULL);
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path = get_env_value(shell->envp, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		path = join_path(paths[i], cmd);
		if (path && access(path, X_OK) == 0)
			return (free_split(paths), path);
		free(path);
	}
	free_split(paths);
	return (NULL);
}
