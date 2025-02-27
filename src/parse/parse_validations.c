/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_validations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 22:21:25 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/27 12:27:35 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int validate_syntax(t_token *tokens)
{
    t_token *current = tokens;

    while (current)
    {
        if (current->type == TOKEN_PIPE)
        {
            if (!current->next || current->next->type == TOKEN_PIPE)
                return (0);
        }
        else if (is_redirection(current->type))
        {
            if (!current->next || current->next->type != TOKEN_WORD)
                return (0);
        }
        current = current->next;
    }
    return (1);
}