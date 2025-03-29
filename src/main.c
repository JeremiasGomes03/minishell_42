/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:29 by jerda-si          #+#    #+#             */
/*   Updated: 2025/03/29 02:42:05 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

static void	check_and_execute_exit(t_cmd_list *cmd_list)
{
	if (!cmd_list || !cmd_list->head || !cmd_list->head->args
		|| !cmd_list->head->args[0])
		return ;
	if (ft_strcmp(cmd_list->head->args[0], "exit") == 0)
		builtin_exit(cmd_list->head);
}

static int	check_syntax_errors(char *input, t_shell *shell)
{
	char	*trimmed;

	trimmed = trim_whitespace(input);
	if (ft_strcmp(trimmed, "$PATH") == 0)
	{
		printf("%s\n", get_envp(shell, "PATH"));
		free(input);
		return (1);
	}
	if (ends_with_pipe(trimmed))
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		free(input);
		return (1);
	}
	return (0);
}

static void	process_tokens_and_execute(char *input, t_shell *shell)
{
	t_token	*tokens;

	tokens = tokenize_input(input);
	if (!validate_syntax(tokens))
	{
		free_tokens(tokens);
		free(input);
		return ;
	}
	expander(&tokens, shell);
	shell->cmd_list = parse_tokens(tokens, shell);
	if (!shell->cmd_list)
	{
		free(input);
		return ;
	}
	check_and_execute_exit(shell->cmd_list);
	execute_pipeline(shell->cmd_list, shell);
	free_cmd_list(shell->cmd_list);
	shell->cmd_list = NULL;
	free(input);
}

static void	process_command(char *input, t_shell *shell)
{
	if (handle_variable_expansion(input, shell))
		return ;
	process_tokens_and_execute(input, shell);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.envp = dup_envp(envp);
	shell.exit_status = 0;
	shell.cmd_list = NULL;
	shell.tokens = NULL;
	setup_signals();
	while (1)
	{
		input = readline("\x1b[1m\x1b[92mminishell\x1b[37m>\x1b[0m ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		if (check_syntax_errors(input, &shell) || !validate_input(input))
			continue ;
		process_command(input, &shell);
	}
	free_envp(shell.envp);
	return (0);
}
