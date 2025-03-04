/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:25 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/04 19:06:21 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	expand_command_args(t_cmd_node *cmd, t_shell *shell)
{
	int		i;
	char	*expanded_arg;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '$'))
		{
			expanded_arg = expand_variables(cmd->args[i], shell);
			if (!expanded_arg)
			{
				printf("Erro ao expandir variáveis no argumento: %s\n",
					cmd->args[i]);
				return ;
			}
			free(cmd->args[i]);
			cmd->args[i] = expanded_arg;
		}
		i++;
	}
}

static void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	execute_child(t_cmd_node *cmd)
{
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
	perror("execvp");
	exit(EXIT_FAILURE);
}

void execute_command(t_cmd_node *cmd, t_shell *shell)
{
    pid_t pid;
    int status;

    if (!cmd || !cmd->args || !cmd->args[0])
        return;
    if (cmd->args[0][0] == '$' && cmd->args[0][1] != '\0')
    {
        char *var_value = get_envp(shell, cmd->args[0]);

        if (var_value)
        {
            printf("%s\n", var_value);
            free(var_value);
            shell->exit_status = 0;
        }
        else
        {
            shell->exit_status = 1;
        }
        return;
    }
    expand_command_args(cmd, shell);
    //Estrutura dos builtins
    // if (is_builtin(cmd->args[0]))
    // {
    //     execute_builtin(cmd, shell);
    //     return;
    // }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        setup_child_signals();
        execute_child(cmd);
    }
    else
    {
        waitpid(pid, &status, 0);
        shell->exit_status = WEXITSTATUS(status);
    }
}

void	execute_pipeline(t_cmd_list *cmd_list, t_shell *shell)
{
	int			pipefd[2];
	t_cmd_node	*cmd;
	int			prev_pipe_in;

	cmd = cmd_list->head;
	prev_pipe_in = -1;
	while (cmd)     
	{
		if (cmd->next && pipe(pipefd) == -1)
		{
			perror("pipe");
			return ;
		}
		if (prev_pipe_in != -1)
			cmd->in_fd = prev_pipe_in;
		if (cmd->next)
			cmd->out_fd = pipefd[1];
		execute_command(cmd, shell);
		if (prev_pipe_in != -1)
			close(prev_pipe_in);
		if (cmd->next)
		{
			close(pipefd[1]);
			prev_pipe_in = pipefd[0];
		}
		cmd = cmd->next;
	}
	if (prev_pipe_in != -1)
		close(prev_pipe_in);
}
    



