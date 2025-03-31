/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:53:46 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/31 01:15:55 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_quotes(char c, int quotes)
{
	if (c == '\"' && quotes != 2)
	{
		if (quotes == 1)
			return (0);
		else
			return (1);
	}
	if (c == '\'' && quotes != 1)
	{
		if (quotes == 2)
			return (0);
		else
			return (2);
	}
	return (quotes);
}

void	expander(t_token **tokens, t_shell *shell)
{
	t_token	*current;
	char	*expanded;

	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded = expand_variables(current->value,
					current->quote_type, shell);
			free(current->value);
			current->value = expanded;
		}
		current = current->next;
	}
}

char	*ft_strjoin_with_free(char *s1, const char *s2, int free_s1)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	if (free_s1)
		free(s1);
	return (result);
}

char	*ft_strjoin_char(char *s1, char c)
{
	char	*result;
	size_t	len;
	size_t	i;

	if (s1)
		len = ft_strlen(s1);
	else
		len = 0;
	result = malloc(len + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = s1[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	free(s1);
	return (result);
}

int	ft_findchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}
