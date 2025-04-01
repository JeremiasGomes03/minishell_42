/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:54:15 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/31 23:55:10 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*accumulate_content(char *content, char *line)
{
	char	*temp_content;
	char	*final_content;

	temp_content = ft_strjoin(content, line);
	if (!temp_content)
	{
		free(content);
		free(line);
		exit(EXIT_FAILURE);
	}
	final_content = ft_strjoin(temp_content, "\n");
	free(temp_content);
	if (!final_content)
	{
		free(line);
		exit(EXIT_FAILURE);
	}
	free(content);
	return (final_content);
}

char	*read_input_line(void)
{
	char	*line;

	line = readline("heredoc> ");
	if (!line)
		return (NULL);
	return (line);
}

static char get_random_char(void)
{
    const char *charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static unsigned long counter = 0;
    size_t charset_len = 62;
    
    counter++;
    return charset[counter % charset_len];
}

static void	generate_temp_name(char *template)
{
	int		i;
	char	*ptr;

	ptr = template + strlen(template) - 6;
	i = 0;
	while (i < 6)
	{
		ptr[i] = get_random_char();
		i++;
	}
}

int	my_mkstemp(char *template)
{
	int	fd;

	generate_temp_name(template);
	fd = open(template, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (fd);
}
