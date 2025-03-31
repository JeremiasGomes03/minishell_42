/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:04:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/31 01:00:39 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	check_pipe_syntax(t_token *tokens, t_shell *shell,
	t_token *original_tokens)
{
	if (tokens && tokens->type == TOKEN_PIPE)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		free_tokens(&original_tokens);
		shell->exit_status = 258;
	}
}

static void	init_cmd_list(t_cmd_list **cmd_list)
{
	*cmd_list = malloc(sizeof(t_cmd_list));
	if (!*cmd_list)
		exit_with_error("Failed to allocate memory");
	(*cmd_list)->head = NULL;
	(*cmd_list)->tail = NULL;
}

static int	handle_token_pipe(t_token **tokens)
{
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		return (1);
	}
	return (0);
}

static void	process_tokens(t_token **tokens, t_cmd_list *cmd_list,
	t_shell *shell, int *valid_commands)
{
	t_cmd_node	*current;
	t_token		*prev;

	while (*tokens && (*tokens)->type != TOKEN_EOF)
	{
		prev = *tokens;
		current = parse_command(tokens, shell);
		if (current)
			handle_parse_command(cmd_list, current, valid_commands);
		if (handle_token_pipe(tokens))
			continue ;
		if (*tokens == prev)
			*tokens = (*tokens)->next;
	}
}

t_cmd_list	*parse_tokens(t_token *tokens, t_shell *shell)
{
	t_cmd_list	*cmd_list;
	int			valid_commands;

	check_pipe_syntax(tokens, shell, tokens);
	if (shell->exit_status == 258)
		return (NULL);
	valid_commands = 0;
	init_cmd_list(&cmd_list);
	process_tokens(&tokens, cmd_list, shell, &valid_commands);
	if (valid_commands == 0)
	{
		free_cmd_list(cmd_list);
		shell->exit_status = 1;
		return (NULL);
	}
	return (cmd_list);
}
