/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:06:00 by jeremias          #+#    #+#             */
/*   Updated: 2025/03/15 18:05:21 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_sigint(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
    }
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_eof(void)
{
	printf("exit\n");
	exit(EXIT_SUCCESS);
}