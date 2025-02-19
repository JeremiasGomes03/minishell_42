/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:30:04 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/19 18:30:50 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

static int	process_token_expansions(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (contains_variable(tokens[i]))
		{
			if (!expand_variables(&tokens[i]))
				return (0);
		}
		if (contains_wildcards(tokens[i]))
		{
			if (!expand_wildcards(&tokens[i]))
				return (0);
		}
		i++;
	}
	return (1);
}

static int	validate_command_syntax(t_cmd_list *cmd)
{
	if (has_redirections(cmd->cmd))
	{
		if (!validate_redirections(cmd->cmd))
		{
			handle_error("Redirection error");
			return (0);
		}
	}
	if (has_quotes(cmd->cmd))
	{
		if (!validate_quotes(cmd->cmd))
		{
			handle_error("Unclosed quotes");
			return (0);
		}
	}
	return (1);
}

static t_cmd_node	*create_command_node(t_cmd_list *cmd)
{
	t_cmd_node	*node;

	node = create_cmd_node();
	if (!node)
		return (NULL);
	node->cmd = get_command(cmd->cmd);
	node->args = get_arguments(cmd->cmd);
	if (has_redirections(cmd->cmd))
		node->redirects = process_redirections(cmd->cmd);
	return (node);
}

static t_cmd_tree	*build_command_tree(t_cmd_list *commands)
{
	t_cmd_tree	*cmd_tree;
	t_cmd_node	*node;

	cmd_tree = create_tree();
	if (!cmd_tree)
		return (NULL);
	while (commands)
	{
		node = create_command_node(commands);
		if (!node || !add_to_tree(cmd_tree, node))
		{
			free_cmd_tree(cmd_tree);
			if (node)
				free_cmd_node(node);
			return (NULL);
		}
		commands = commands->next;
	}
	return (cmd_tree);
}

static int	validate_all_commands(t_cmd_list *commands)
{
	t_cmd_list	*current;

	current = commands;
	while (current)
	{
		if (!validate_command_syntax(current))
			return (0);
		current = current->next;
	}
	return (1);
}

t_cmd_tree	*parse_input(char *input_line)
{
	t_token		*tokens;
	t_cmd_list	*commands;
	t_cmd_tree	*cmd_tree;

	if (!input_line || !*input_line)
		return (NULL);
	tokens = tokenization_loop(input_line);
	if (!tokens)
		return (handle_error("Tokenization error"), NULL);
	if (!process_token_expansions(tokens))
		return (free_tokens(tokens), NULL);
	commands = split_by_pipes(tokens);
	if (!commands || !validate_all_commands(commands))
	{
		free_tokens(tokens);
		if (commands)
			free_commands(commands);
		return (NULL);
	}
	cmd_tree = build_command_tree(commands);
	free_tokens(tokens);
	free_commands(commands);
	return (cmd_tree);
}
