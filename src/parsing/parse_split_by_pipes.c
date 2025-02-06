/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_split_by_pipes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:10:31 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/06 16:20:47 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


t_cmd_list split_by_pipes(t_token *tokens)
{
	int	i;
	char **commands;
	char **current_command;
	
	commands = create_empty_command_list();
	current_command = create_empty_command();

	i = 0;
	while (tokens[i])
	{
		if (tokens[i] == PIPE_TOKEN)
		{
			if(!current_command)
				return (NULL);
		}
		i++;
	}
}
// Function split_by_pipes(tokens):
//     # Initialize variables
//     commands = create_empty_command_list()
//     current_command = create_empty_command()
    
//     # Iterate through all tokens
//     for each token in tokens:
//         if token is PIPE_TOKEN:
//             # Add the current command to the list and create a new one
//             if current_command is empty:
//                 return NULL  # Error: empty command before pipe
//             add_command_to_list(commands, current_command)
//             current_command = create_empty_command()
//         else:
//             # Add token to current command
//             add_token_to_command(current_command, token)
    
//     # Add the last command if it exists
//     if current_command is not empty:
//         add_command_to_list(commands, current_command)
//     else if commands is empty:
//         return NULL  # Error: no commands found
    
//     # Validate the command list
//     if commands is empty:
//         return NULL
    
//     return commands

// Function add_token_to_command(command, token):
//     append token to command.tokens
//     increment command.token_count

// Function add_command_to_list(commands, command):
//     append command to commands.list
//     increment commands.count