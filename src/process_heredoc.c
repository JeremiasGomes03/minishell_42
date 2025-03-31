/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:50:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/31 18:39:01 by jeremias         ###   ########.fr       */
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
    int fd;

    signal(SIGINT, SIG_IGN); // Bloqueia SIGINT no pai durante o wait
    waitpid(pid, &status, 0); // Espera o filho terminar
    signal(SIGINT, SIG_DFL); // Restaura comportamento padrão

    // Verifica se o filho foi interrompido por SIGINT
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        cleanup_temp_file(temp_file); // Remove o arquivo temporário
        ft_putstr_fd("\n", STDERR_FILENO); // Mantém o prompt alinhado
        shell->exit_status = 130; // Código 130 = SIGINT
        return (-2); // Retorno especial para interrupção
    }

    fd = open_temp_file_for_reading(temp_file);
    return (fd); // Retorna o fd ou -1 em caso de erro
}

int process_heredoc(t_heredoc *heredoc_data, t_shell *shell)
{
    pid_t pid;
    char *temp_file;
    int fd;

    temp_file = create_temp_file();
    if (!temp_file)
        return (-1);

    pid = fork();
    if (pid == 0) // Processo filho
    {
        setup_heredoc_signals(); // SIGINT agora termina o processo
        fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("open");
            free(temp_file);
            exit(EXIT_FAILURE);
        }
        if (handle_heredoc_child(fd, heredoc_data, shell) != 0)
            exit(EXIT_FAILURE);
        close(fd);
        free(temp_file);
        exit(EXIT_SUCCESS);
    }
    else // Processo pai
    {
        fd = handle_heredoc_parent(pid, temp_file, shell);
        if (fd == -2) // Caso de interrupção por SIGINT
            cleanup_temp_file(temp_file);
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

char	*process_input_heredoc(t_heredoc *heredoc_data, t_shell *shell)
{
	char	*line;
	char	*content;
	char	*expanded_line;

	content = ft_strdup("");
	while (1)
	{
		line = read_input_line();
		if (!line || ft_strcmp(line, heredoc_data->delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (heredoc_data->quote_type == NO_QUOTES)
		{
			expanded_line = expand_variables(line, NO_QUOTES, shell);
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
