/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_contains.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:53:17 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/19 18:55:09 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/minishell.h"

int	contains_variable(t_token *token)
{
	int	i;

	if (!token || !token->value)
		return (0);
	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '$' && token->value[i + 1]
			&& (ft_isalnum(token->value[i + 1]) || token->value[i + 1] == '_'
				|| token->value[i + 1] == '?'))
			return (1);
		i++;
	}
	return (0);
}

int	contains_wildcards(t_token *token)
{
	int	i;

	if (!token || !token->value)
		return (0);
	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '*' || token->value[i] == '?')
			return (1);
		i++;
	}
	return (0);
}

static char	*extract_var_name(const char *str)
{
	char	*name;
	int		i;

	if (!str)
		return (NULL);
	if (str[0] == '?')
	{
		name = ft_strdup("?");
		return (name);
	}
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	name = malloc(sizeof(char) * (i + 1));
	if (!name)
		return (NULL);
	ft_strlcpy(name, str, i + 1);
	return (name);
}

int	expand_variables(t_token **token)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	int		j;

	if (!token || !*token || !(*token)->value)
		return (0);
	result = malloc(sizeof(char) * MAX_TOKEN_SIZE);
	if (!result)
		return (0);
	i = 0;
	j = 0;
	while ((*token)->value[i])
	{
		if (handle_variable_expansion(&i, &j, result, *token) == 0)
			return (free(result), 0);
		else if ((*token)->value[i])
			result[j++] = (*token)->value[i++];
	}
	result[j] = '\0';
	free((*token)->value);
	(*token)->value = result;
	return (1);
}

static int	handle_variable_expansion(int *i, int *j, char *result,
	t_token *token)
{
	char	*var_name;
	char	*var_value;

	if (token->value[*i] == '$' && token->value[*i + 1])
	{
		var_name = extract_var_name(&token->value[*i + 1]);
		if (!var_name)
			return (0);
		if (ft_strcmp(var_name, "?") == 0)
			var_value = get_last_exit_status();
		else
			var_value = getenv(var_name);
		if (var_value)
		{
			ft_strlcpy(&result[*j], var_value, MAX_TOKEN_SIZE - *j);
			*j += ft_strlen(var_value);
		}
		*i += ft_strlen(var_name) + 1;
		free(var_name);
		return (1);
	}
	return (1);
}

static int	match_pattern(const char *pattern, const char *name)
{
	if (*pattern == '\0' && *name == '\0')
		return (1);
	if (*pattern == '*')
	{
		while (*name)
		{
			if (match_pattern(pattern + 1, name))
				return (1);
			name++;
		}
		return (match_pattern(pattern + 1, name));
	}
	if (*pattern == '?' || *pattern == *name)
		return (match_pattern(pattern + 1, name + 1));
	return (0);
}

int	expand_wildcards(t_token **token)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*expanded;
	int				found_match;

	if (!init_wildcard_expansion(token, &expanded, &dir))
		return (0);
	found_match = 0;
	while ((entry = readdir(dir)))
	{
		if (should_expand_entry(entry->d_name, (*token)->value))
		{
			if (found_match)
				ft_strlcat(expanded, " ", MAX_TOKEN_SIZE);
			ft_strlcat(expanded, entry->d_name, MAX_TOKEN_SIZE);
			found_match = 1;
		}
	}
	closedir(dir);
	if (!found_match)
		return (free(expanded),1);
	free((*token)->value);
	(*token)->value = expanded;
	return (1);
}

static int	init_wildcard_expansion(t_token **token, char **expanded,
	DIR **dir)
{
	if (!token || !*token || !(*token)->value)
		return (0);
	*expanded = malloc(sizeof(char) * MAX_TOKEN_SIZE);
	if (!*expanded)
		return (0);
	(*expanded)[0] = '\0';
	*dir = opendir(".");
	if (!*dir)
	{
		free(*expanded);
		return (0);
	}
	return (1);
}

static int	should_expand_entry(char *entry_name, char *pattern)
{
	if (!match_pattern(pattern, entry_name))
		return (0);
	if (entry_name[0] == '.' && pattern[0] != '.')
		return (0);
	return (1);
}