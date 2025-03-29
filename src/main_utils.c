/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 02:32:18 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/29 14:31:40 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*trim_whitespace(char *str)
{
	char	*end;

	if (str == NULL || *str == '\0')
		return (str);
	while (ft_isspace((unsigned char)*str))
		str++;
	if (*str == '\0')
		return (str);
	end = str + ft_strlen(str) - 1;
	while (end > str && ft_isspace((unsigned char)*end))
		end--;
	*(end + 1) = '\0';
	return (str);
}

int	ends_with_pipe(const char *input)
{
	int	len;

	if (!input)
		return (0);
	len = ft_strlen(input);
	while (len > 0 && ft_isspace((unsigned char)input[len - 1]))
		len--;
	return (len > 0 && input[len - 1] == '|');
}

int	validate_input(char *input)
{
	if (ft_strnstr(input, "&&", ft_strlen(input))
		|| ft_strnstr(input, "||", ft_strlen(input))
		|| ft_strnstr(input, "\\", ft_strlen(input)))
	{
		write(2,
			"minishell: unexpected token `&&' or `||' or `\\'\n", 49);
		free(input);
		return (0);
	}
	return (1);
}

int	handle_variable_expansion(char *input, t_shell *shell)
{
	char	*expanded;

	if (input[0] != '$' || ft_strchr(input, ' ') || input[1] == '\0')
		return (0);
	expanded = expand_variables(input, shell);
	if (expanded && *expanded)
		printf("%s\n", expanded);
	else
		printf("\n");
	free(expanded);
	free(input);
	return (1);
}

t_heredoc	*init_heredoc_data(t_token *token)
{
	t_heredoc	*heredoc_data;

	heredoc_data = malloc(sizeof(t_heredoc));
	if (!heredoc_data)
		exit_with_error("malloc");
	if ((token->quote_type == SINGLE_QUOTES || token->quote_type
			== DOUBLE_QUOTES)
		&& ft_strlen(token->value) >= 2)
	{
		heredoc_data->quote_type = token->quote_type;
		heredoc_data->delimiter = ft_substr(token->value, 1,
				ft_strlen(token->value) - 2);
	}
	else
	{
		heredoc_data->quote_type = NO_QUOTES;
		heredoc_data->delimiter = ft_strdup(token->value);
	}
	return (heredoc_data);
}
