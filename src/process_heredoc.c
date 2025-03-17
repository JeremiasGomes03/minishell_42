/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:50:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/17 20:47:52 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int process_heredoc(t_heredoc *heredoc_data, t_shell *shell)
{
    char    *temp_file;
    char    *content;
    int     fd;

    temp_file = create_temp_file();
    content = process_input_heredoc(heredoc_data, shell);
    fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		free(content);
		free(temp_file);
		exit(EXIT_FAILURE);
	}
	write_content_to_temp_file(fd, content);
	free(content);
	close(fd);
	fd = open_temp_file_for_reading(temp_file);
	return (fd);
}

char	*create_temp_file(void)
{
	char	*temp_file;
	int		fd;

	temp_file = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
	if (!temp_file)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	fd = my_mkstemp(temp_file);
	if (fd == -1)
	{
		perror("mkstemp");
		free(temp_file);
		exit(EXIT_FAILURE);
	}
	close(fd);
	return (temp_file);
}

char *process_input_heredoc(t_heredoc *heredoc_data, t_shell *shell)
{
    char    *line;
    char    *content;
    char    *expanded_line;

    content = ft_strdup("");
    while (1)
    {
        line = read_input_line();
        if (!line || ft_strcmp(line, heredoc_data->delimiter) == 0)
        {
            free(line);
            break;
        }
        if (heredoc_data->quote_type != SINGLE_QUOTES)
        {
            expanded_line = expand_variables(line, shell);
			if (!expanded_line)
				return (ft_strdup(""));
            free(line);
            line = expanded_line;
        }
        content = accumulate_content(content, line);
        free(line);
    }
    return (content);
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
		perror("open");
		unlink(temp_file);
		free(temp_file);
		exit(EXIT_FAILURE);
	}
	unlink(temp_file);
	free(temp_file);
	return (fd);
}




