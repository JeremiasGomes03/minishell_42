/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamachad <lamachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 21:39:49 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/05 20:20:07 by lamachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	append_command(t_cmd_list *cmd_list, t_cmd_node *cmd)
{
	if (!cmd_list->head)
	{
		cmd_list->head = cmd;
		cmd_list->tail = cmd;
	}
	else
	{
		cmd_list->tail->next = cmd;
		cmd_list->tail = cmd;
	}
}

void	free_cmd_list(t_cmd_list *cmd_list)
{
	t_cmd_node	*current;
	t_cmd_node	*next;

	current = cmd_list->head;
	while (current)
	{
		next = current->next;
		free(current->args);
		free(current);
		current = next;
	}
	free(cmd_list);
}

t_cmd_node	*create_cmd_node(void)
{
	t_cmd_node	*node;

	node = malloc(sizeof(t_cmd_node));
	if (!node)
		exit_with_error("Failed to allocate memory");
	node->args = malloc(sizeof(char *));
	if (!node->args)
		exit_with_error("Failed to allocate memory");
	node->args[0] = NULL;
	node->in_fd = STDIN_FILENO;
	node->out_fd = STDOUT_FILENO;
	node->next = NULL;
	return (node);
}

void	add_arg_to_cmd(t_cmd_node *cmd, char *arg)
{
	int	i;

	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	cmd->args = realloc(cmd->args, (i + 2) * sizeof(char *));
	if (!cmd->args)
		exit_with_error("Failed to allocate memory");
	cmd->args[i] = arg;
	cmd->args[i + 1] = NULL;
}

int	ft_isnumeric(char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}