/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:38:21 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/28 14:42:53 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_free_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
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
