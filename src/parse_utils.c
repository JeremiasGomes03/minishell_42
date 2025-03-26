/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 21:39:49 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/26 19:16:57 by jeremias         ###   ########.fr       */
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
	t_cmd_node	*cmd;
	t_cmd_node	*next;
	int			i;

	cmd = cmd_list->head;
	while (cmd)
	{
		next = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		free(cmd);
		cmd = next;
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
	int		i;
	char	**tmp;

	if (!arg)
		return ;
	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	tmp = realloc(cmd->args, (i + 2) * sizeof(char *));
	if (!tmp)
		exit_with_error("Failed to allocate memory for cmd->args");
	cmd->args = tmp;
	cmd->args[i] = ft_strdup(arg);
	if (!cmd->args[i])
		exit_with_error("Failed to duplicate argument");
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

void	free_cmd_node(t_cmd_node *cmd)	
{
	if (cmd)
	{
		if (cmd->args)
			ft_free_array(cmd->args);
		if (cmd->in_fd != STDIN_FILENO && cmd->in_fd != -1)
			close(cmd->in_fd);
		if (cmd->out_fd != STDOUT_FILENO && cmd->out_fd != -1)
			close(cmd->out_fd);
		free(cmd);
	}
}
