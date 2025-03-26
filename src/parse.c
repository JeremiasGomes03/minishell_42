/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:04:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/26 18:09:17 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	check_pipe_syntax(t_token *tokens, t_shell *shell,
	t_token *original_tokens)
{
	if (tokens && tokens->type == TOKEN_PIPE)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		free_tokens(original_tokens);
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

static t_cmd_node	*handle_redirection_error(t_token **tokens, t_shell *shell)
{
	char	*filename;

	if (*tokens && (*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
	{
		filename = (*tokens)->next->value;
		fprintf(stderr,
			"minishell: redirecionamento mal formado: `%s'\n", filename);
	}
	else
	{
		fprintf(stderr, "minishell: erro de redirecionamento\n");
	}
	shell->exit_status = 1;
	return (0);
}

t_cmd_list	*parse_tokens(t_token *tokens, t_shell *shell)
{
    t_cmd_list		*cmd_list;
    t_cmd_node		*current;
    t_token			*original_tokens;
    int				valid_commands;

	valid_commands = 0;
    original_tokens = tokens;
    check_pipe_syntax(tokens, shell, original_tokens);
    if (shell->exit_status == 258)
        return (NULL);
    init_cmd_list(&cmd_list);
    current = NULL;
    while (tokens && tokens->type != TOKEN_EOF)
    {
        current = parse_command(&tokens, shell);
        if (current)
        {
            append_command(cmd_list, current);
            valid_commands++;
        }

        if (handle_token_pipe(&tokens))
            continue;
    }
    if (valid_commands == 0)
    {
        free_cmd_list(cmd_list);
        shell->exit_status = 1;
        return (NULL);
    }
    return (cmd_list);
}

t_cmd_node *parse_command(t_token **tokens, t_shell *shell)
{
    t_cmd_node *cmd;
    int redirection_error = 0;

    if (*tokens && is_redirection((*tokens)->type))
        return (handle_redirection_error(tokens, shell));

    cmd = create_cmd_node();
    while (*tokens && (*tokens)->type != TOKEN_PIPE && (*tokens)->type != TOKEN_EOF)
    {
        if (is_redirection((*tokens)->type))
        {
            if (!parse_redirection(cmd, tokens, shell))
            {
                redirection_error = 1;
                break;
            }
        }
        else
        {
            add_arg_to_cmd(cmd, (*tokens)->value);
            *tokens = (*tokens)->next;
        }
    }
    if (redirection_error && (!cmd->args || !cmd->args[0]))
    {
        free_cmd_node(cmd);
        return (NULL);
    }
    return (cmd);
}
