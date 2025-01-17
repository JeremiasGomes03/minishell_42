/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:29 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/17 19:29:33 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(void)
{
	char *line;
	char **tokens;
	int i;
	
	i = 0;
	while (1)
	{
		line = readline("TBShell>");
		if (!line)
			 printf("exit\n");
		//init_token()
		tokens = ft_split(line, ' ');
		while (tokens[i])
		{
			printf("Token:%s\n", tokens[i]);
			i++;
		}
		if (*line)
            add_history(line);
		i = 0;
	}
	
}
