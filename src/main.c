/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:29 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/22 17:12:21 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/token.h"

// int main(void) {
//     char *input;
    
//     while (1) {
//         input = readline("TBShell> ");
//         if (!input) {
//             printf("exit\n");
//             break;
//         }
//         init_tokenizer(input);
//         if (*input) {
//             add_history(input);
//         }
//         free(input);
//     }
//     return 0;
// }

void print_token_type(t_token_type type)
{
    switch (type)
    {
        case TOKEN_WORD: printf("WORD"); break;
        case TOKEN_PIPE: printf("PIPE"); break;
        case TOKEN_REDIR_IN: printf("REDIR_IN"); break;
        case TOKEN_REDIR_OUT: printf("REDIR_OUT"); break;
        case TOKEN_APPEND: printf("APPEND"); break;
        case TOKEN_QUOTE: printf("QUOTE"); break;
        case TOKEN_DQUOTE: printf("DQUOTE"); break;
        default: printf("UNKNOWN"); break;
    }
}

void print_tokens(t_token *tokens)
{
    if (!tokens)
    {
        printf("No tokens to print (NULL)\n");
        return;
    }

    t_token *current = tokens;
    int i = 0;

    while (current)
    {
        if (!current->value)
        {
            printf("Token %d has NULL value\n", i);
            return;
        }
        printf("Token %d:\n", i++);
        printf("  Value: [%s]\n", current->value);
        printf("  Type:  ");
        print_token_type(current->type);
        printf("\n");
        current = current->next;
    }
}

int main(void)
{
    char *input;

    while (1) {
        // Obtém a entrada do usuário
        input = readline("TBShell> ");
        if (!input) {
            printf("exit\n");
            break;
        }

        // Adiciona a entrada ao histórico (opcional)
        add_history(input);

        // Debug print
        printf("\nTesting input: [%s]\n", input);
        printf("------------------------\n");

        // Processa a entrada com tokenização
        printf("Calling tokenization_loop...\n");
        t_tokenizer *tokenizer = tokenization_loop(input);
        if (!tokenizer) {
            printf("Error: tokenizer is NULL\n");
            free(input); // Libera a memória alocada para a entrada
            continue;
        }

        // Debug print
        printf("Tokenizer created successfully\n");

        if (!tokenizer->tokens) {
            printf("Warning: No tokens were created (tokenizer->tokens is NULL)\n");
        }

        // Imprime os tokens gerados
        print_tokens(tokenizer->tokens);

        // Libera a memória alocada
        printf("Freeing tokens...\n");
        free_all_tokens(tokenizer);
        free(tokenizer); // Libera o próprio tokenizer após liberar os tokens
        free(input);

        printf("Memory freed successfully\n");
    }

    return 0;
}
