/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:50:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/04/01 00:06:02 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_heredoc_child(int fd, t_heredoc *hd, t_shell *shell)
{
	char	*content;
	char	*line;

	content = ft_strdup("");
	while (1)
	{
		line = read_input_line();
		if (!line || ft_strcmp(line, hd->delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (hd->quote_type == NO_QUOTES)
			line = expand_variables(line, NO_QUOTES, shell);
		content = accumulate_content(content, line);
		free(line);
	}
	write_content_to_temp_file(fd, content);
	free(content);
	close(fd);
	return (0);
}

static int handle_heredoc_parent(pid_t pid, char *temp_file, t_shell *shell)
{
    int status;
    int fd = -1;
    
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        shell->exit_status = 130;
        ft_putstr_fd("\n", STDERR_FILENO);
        return (-2);
    }
    fd = open(temp_file, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return (-1);
    }
    return (fd);
}

int process_heredoc(t_heredoc *heredoc_data, t_shell *shell)
{
    pid_t pid;
    char *temp_file;
    int fd = -1;
    void (*old_handler)(int);

    temp_file = create_temp_file();
    if (!temp_file)
        return (-1);
    old_handler = signal(SIGINT, SIG_IGN);
    pid = fork();
    if (pid == -1) {
        signal(SIGINT, old_handler);
        free(temp_file);
        return (-1);
    }
    if (pid == 0)
    {
        setup_heredoc_signals();
        fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("open");
            free(temp_file);
            exit(EXIT_FAILURE);
        }
        if (handle_heredoc_child(fd, heredoc_data, shell) != 0)
            exit(EXIT_FAILURE);
        exit(EXIT_SUCCESS);
    }
    else
    {
        fd = handle_heredoc_parent(pid, temp_file, shell);
        signal(SIGINT, old_handler);
        if (fd == -2) {
            shell->exit_status = 130;
            unlink(temp_file);
            free(temp_file);
            rl_on_new_line();
            return (-1);
        }
        free(temp_file);
        return (fd);
    }
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
