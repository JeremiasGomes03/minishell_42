/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamachad <lamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:29 by jerda-si          #+#    #+#             */
/*   Updated: 2025/03/10 18:51:37 by lamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>


// static	void	process_input(char *input, t_token **tokens, t_cmd_list **cmd_list)
// {
// 	*tokens = tokenize_input(input);
// 	if (!validate_syntax(*tokens))
// 	{
// 		printf("Erro de sintaxe\n");
// 		free(input);
// 		free_tokens(*tokens);
// 		*tokens = NULL;
// 		return ;
// 	}
// 	*cmd_list = parse_tokens(*tokens);
// }

// static	void check_and_execute_exit(t_cmd_list *cmd_list)
// {
// 	if (cmd_list->head && cmd_list->head->args && ft_strcmp(cmd_list->head->args[0], "exit") == 0)
// 	{
// 		builtin_exit(cmd_list->head);
// 	}
// }

// static	void execute_and_cleanup(t_cmd_list *cmd_list, char *input, t_token *tokens, t_shell *shell)
// {
// 	if (cmd_list)
// 	{
// 		execute_pipeline(cmd_list, shell);
// 		free_cmd_list(cmd_list);
// 	}
// 	free(input);
// 	free_tokens(tokens);
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	char		*input;
// 	t_shell		shell;

// 	(void)argc;
// 	(void)argv;

// 	shell.envp = envp;
// 	shell.exit_status = 0;
// 	setup_signals();
// 	while (1)
// 	{
// 		input = readline("minishell> ");
// 		if (!input)
// 			break ;
// 		if (*input)
// 			add_history(input);
// 		process_input(input, &shell.tokens, &shell.cmd_list);
// 		if (shell.cmd_list)
// 			check_and_execute_exit(shell.cmd_list);
// 		execute_and_cleanup(shell.cmd_list, input, shell.tokens, &shell);
// 	}
// 	return (0);
// }



