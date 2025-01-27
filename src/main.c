/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:29 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/27 20:05:22 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(void) {
    char *input;
    
    while (1) {
        input = readline("TBShell% ");
        if (!input) {
            printf("exit\n");
            break;
        }
        init_tokenizer(input);
        if (*input) {
            add_history(input);
        }
        free(input);
    }
    return 0;
}
