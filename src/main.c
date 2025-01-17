/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:29 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/14 19:16:32 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
	char *line;
	char **tokens;
	int i;
	
	i = 0;
	//enquanto o shell estiver rodando
	while (1)
	{
		line = readline("TBShell>");
		if (!line)
			 printf("exit\n");
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