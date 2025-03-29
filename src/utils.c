/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:18:05 by jerda-si          #+#    #+#             */
/*   Updated: 2025/03/28 22:38:56 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exit_with_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next || current->next->type == TOKEN_PIPE)
				return (printf("Syntax error near unexpected token `|'\n"), 0);
		}
		else if (is_redirection(current->type))
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				printf("Syntax error unexpected token `%s'\n", current->value);
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

char	**ft_arrdup(char **arr)
{
	char	**new;
	int		i;

	i = 0;
	while (arr[i])
		i++;
	new = malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (arr[++i])
		new[i] = ft_strdup(arr[i]);
	new[i] = NULL;
	return (new);
}

int	ft_isspace(int c)
{
	return (c == ' '
		|| c == '\f' || c == '\n' || c == '\r'
		|| c == '\t' || c == '\v');
}
