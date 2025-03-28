/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:08:12 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/28 14:54:40 by jeremias         ###   ########.fr       */
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

static int	handle_heredoc(t_cmd_node *cmd, t_token **tokens, t_shell *shell)
{
	char	*delimiter;
	int		quote_type;

	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
	{
		shell->exit_status = 1;
		fprintf(stderr, "minishell: heredoc sem delimitador\n");
		return (0);
	}
	delimiter = (*tokens)->next->value;
	quote_type = (*tokens)->next->quote_type;
	if (!expand_redirectio_fname(&delimiter, quote_type, shell))
		return (0);
	cmd->in_fd = process_heredoc(&(t_heredoc){delimiter, quote_type}, shell);
	free(delimiter);
	*tokens = (*tokens)->next->next;
	return (cmd->in_fd != -1);
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
	t_token_type	type;
	int				success;

	type = (*tokens)->type;
	if (type == TOKEN_HEREDOC)
		success = handle_heredoc(cmd, tokens, shell);
	else if (type == TOKEN_REDIR_IN)
		success = handle_redir_in(cmd, tokens, shell);
	else if (type == TOKEN_REDIR_OUT)
		success = handle_redir_out(cmd, tokens, 0, shell);
	else if (type == TOKEN_APPEND)
		success = handle_redir_out(cmd, tokens, 1, shell);
	else
		success = 1;
	return (success);
}
