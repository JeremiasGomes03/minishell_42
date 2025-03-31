/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:25:41 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/31 16:00:18 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_cmd_node	*handle_redirection_error(t_token **tokens, t_shell *shell)
{
	char	*filename;

	if (*tokens && (*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
	{
		if (is_redirection((*tokens)->next->type))
		{
			filename = (*tokens)->next->value;
			fprintf(stderr,
				"minishell: redirecionamento mal formado: `%s'\n", filename);
			*tokens = (*tokens)->next->next;
			shell->exit_status = 1;
			return (NULL);
		}
	}
	return (NULL);
}

// static int	process_redirection(t_cmd_node *cmd,
// 	t_token **tokens, t_shell *shell)
// {
// 	if (is_redirection((*tokens)->type))
// 	{
// 		if (!parse_redirection(cmd, tokens, shell))
// 			return (0);
// 	}
// 	return (0);
// }

t_cmd_node *parse_command(t_token **tokens, t_shell *shell)
{
    t_cmd_node *cmd;
    int redirection_error;

    redirection_error = 0;
    if (handle_redirection_error(tokens, shell))
        return (NULL);
    cmd = create_cmd_node();
    while (*tokens && (*tokens)->type != TOKEN_PIPE && (*tokens)->type != TOKEN_EOF)
    {
        if (is_redirection((*tokens)->type))
        {
            if (!parse_redirection(cmd, tokens, shell))
            {
                redirection_error = 1;
                break ;
            }
            continue ;
        }
        add_arg_to_cmd(cmd, (*tokens)->value);
        *tokens = (*tokens)->next;
    }
    if (redirection_error && (!cmd->args || !cmd->args[0]))
        return (free_cmd_node(cmd), NULL);
    return (cmd);
}
