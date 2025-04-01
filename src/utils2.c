/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 21:26:44 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/31 23:55:25 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void cleanup_temp_file(char *temp_file, int fd)
{
    if (fd >= 0)
        close(fd);
    if (temp_file)
        unlink(temp_file);
}

void	write_content_to_temp_file(int fd, char *content)
{
	ssize_t	written;

	if (ft_strlen(content) > 0)
	{
		written = write(fd, content, ft_strlen(content));
		if (written < 0)
		{
			perror("write");
			free(content);
			close(fd);
			exit(EXIT_FAILURE);
		}
	}
}

int	open_temp_file_for_reading(char *temp_file)
{
	int	fd;

	fd = open(temp_file, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		cleanup_temp_file(temp_file, -1);
		return (-1);
	}
	unlink(temp_file);
	free(temp_file);
	return (fd);
}

void	handle_parse_command(t_cmd_list *cmd_list, t_cmd_node *current,
	int *valid_commands)
{
	if (current->args && current->args[0])
	{
		append_command(cmd_list, current);
		(*valid_commands)++;
		cmd_list->size = *valid_commands;
	}
	else if (has_non_heredoc_redirs(current))
	{
		if (current->in_fd != STDIN_FILENO)
			close(current->in_fd);
		if (current->out_fd != STDOUT_FILENO)
			close(current->out_fd);
		free_cmd_node(current);
	}
	else
		free_cmd_node(current);
}
