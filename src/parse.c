/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:04:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/14 21:47:30 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd_list *parse_tokens(t_token *tokens, t_shell *shell)
{
	t_cmd_list	*cmd_list;
	t_cmd_node	*current;

	cmd_list = malloc(sizeof(t_cmd_list));
	if (!cmd_list)	
		exit_with_error("Failed to allocate memory");
	cmd_list->head = NULL;
	cmd_list->tail = NULL;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		current = parse_command(&tokens, shell); 
		append_command(cmd_list, current);
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (cmd_list);
}


t_cmd_node *parse_command(t_token **tokens, t_shell *shell)
{
	t_cmd_node	*cmd;

	cmd = create_cmd_node();
	while (*tokens && (*tokens)->type != TOKEN_PIPE
		&& (*tokens)->type != TOKEN_EOF)
	{
		if (is_redirection((*tokens)->type))
			parse_redirection(cmd, tokens, shell);
		else
		{
			add_arg_to_cmd(cmd, (*tokens)->value);
			*tokens = (*tokens)->next;
		}
	}
	return (cmd);
}
	
		