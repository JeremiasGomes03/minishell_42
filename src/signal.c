/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:06:00 by jeremias          #+#    #+#             */
/*   Updated: 2025/02/27 21:54:59 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Handler para Ctrl+C (SIGINT)
void handle_sigint(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n"); // Nova linha
        rl_on_new_line(); // Reseta o readline
        rl_replace_line("", 0); // Limpa a linha atual
        rl_redisplay(); // Reexibe o prompt
    }
}

// Configura os handlers de sinais
void setup_signals(void)
{
    signal(SIGINT, handle_sigint); // Ctrl+C
    signal(SIGQUIT, SIG_IGN); // Ignora Ctrl+
}

// Trata Ctrl+D (EOF)
void handle_eof(void)
{
    printf("exit\n"); // Exibe "exit" ao receber Ctrl+D
    exit(EXIT_SUCCESS); // Encerra o Minishell
}