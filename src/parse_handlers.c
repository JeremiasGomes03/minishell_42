/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:08:12 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/29 14:30:44 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:08:12 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/25 22:17:10 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	expand_redirectio_fname(char **filename, int quote_type,
	t_shell *shell)
{
	char	*expanded;
	char	*temp;

	if (quote_type != 2)
	{
		expanded = expand_variables(*filename, shell);
		if (!expanded)
			return (0);
	}
	else
	{
		expanded = ft_strdup(*filename);
	}
	if (expanded[0] == '\1')
	{
		temp = ft_strdup(expanded + 1);
		free(expanded);
		if (!temp)
			return (0);
		expanded = temp;
	}
	free(*filename);
	*filename = expanded;
	return (1);
}

static int	handle_heredoc(t_cmd_node *cmd, t_token **token, t_shell *shell)
{
	t_heredoc	*heredoc_data;
	int			fd;

	if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
		return (ft_putstr_fd("minishell: heredoc sem delimitador\n",
				STDERR_FILENO), 0);
	heredoc_data = init_heredoc_data((*token)->next);
	fd = process_heredoc(heredoc_data, shell);
	free(heredoc_data->delimiter);
	free(heredoc_data);
	if (fd == -1)
		return (0);
	cmd->in_fd = fd;
	*token = (*token)->next->next;
	return (1);
}

static int	handle_redir_in(t_cmd_node *cmd, t_token **tokens, t_shell *shell)
{
	char	*filename;
	int		fd;

	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
	{
		shell->exit_status = 1;
		return (0);
	}
	filename = ft_strdup((*tokens)->next->value);
	if (!expand_redirectio_fname(&filename, (*tokens)->next->quote_type, shell))
		return (free(filename), 0);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "minishell: %s: No such file or directory\n", filename);
		shell->exit_status = 1;
		free(filename);
		cmd->in_fd = open("/dev/null", O_RDONLY);
		*tokens = (*tokens)->next->next;
		return (1);
	}
	free(filename);
	cmd->in_fd = fd;
	*tokens = (*tokens)->next->next;
	return (1);
}

static int	handle_redir_out(t_cmd_node *cmd, t_token **tokens,
	int append, t_shell *shell)
{
	char	*filename;
	int		flags;

	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
	{
		shell->exit_status = 1;
		fprintf(stderr, "minishell: redirecionamento sem arquivo\n");
		return (0);
	}
	filename = ft_strdup((*tokens)->next->value);
	if (!expand_redirectio_fname(&filename, (*tokens)->next->quote_type, shell))
		return (free(filename), 0);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
	cmd->out_fd = open(filename, flags, 0644);
	free(filename);
	if (cmd->out_fd == -1)
		return (perror("minishell"), shell->exit_status = 1, 0);
	*tokens = (*tokens)->next->next;
	return (1);
}

int	parse_redirection(t_cmd_node *cmd, t_token **tokens, t_shell *shell)
{
	t_token			*current;
	int				success;
	t_token_type	type;

	current = *tokens;
	success = 1;
	while (current && is_redirection(current->type))
	{
		type = current->type;
		if (type == TOKEN_HEREDOC)
			success = handle_heredoc(cmd, &current, shell);
		else if (type == TOKEN_REDIR_IN)
			success = handle_redir_in(cmd, &current, shell);
		else if (type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
			success = handle_redir_out(cmd, &current, type
					== TOKEN_APPEND, shell);
		if (!success)
			break ;
	}
	*tokens = current;
	return (success);
}
