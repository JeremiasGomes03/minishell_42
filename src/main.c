/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:29 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/27 13:19:23 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




//################################# PARSEAMENTO #############################//////
// #include "../include/minishell.h"
// #include <stdio.h>
// #include <readline/readline.h>
// #include <readline/history.h>
// // Função para imprimir a árvore de comandos (para teste)
// void print_cmd_tree(t_cmd_node *node, int depth) {
//     if (!node)
//         return;

//     // Indentação para melhor visualização
//     for (int i = 0; i < depth; i++)
//         printf("  ");

//     // Imprime o comando e os argumentos
//     printf("Comando: %s\n", node->cmd);
//     if (node->args) {
//         for (int i = 0; node->args[i]; i++) {
//             for (int j = 0; j < depth + 1; j++)
//                 printf("  ");
//             printf("Argumento %d: %s\n", i + 1, node->args[i]);
//         }
//     }

//     // Imprime redirecionamentos
//     if (node->in_fd != STDIN_FILENO)
//         printf("  Redirecionamento de entrada: fd %d\n", node->in_fd);
//     if (node->out_fd != STDOUT_FILENO)
//         printf("  Redirecionamento de saída: fd %d\n", node->out_fd);

//     // Recursão para os nós à esquerda e à direita (pipelines)
//     print_cmd_tree(node->left, depth + 1);
//     print_cmd_tree(node->right, depth + 1);
// }

// int main(void) {
//     char *input;
//     t_tokenizer *tokenizer;
//     t_cmd_tree *cmd_tree;

//     while (1) {
//         // Ler entrada do usuário
//         input = readline("\x1B[34m🐗TBShell\x1B[37m% ");
//         if (!input) {
//             printf("exit\n");
//             break;
//         }

//         // Adicionar entrada ao histórico (se não estiver vazia)
//         if (*input)
//             add_history(input);

//         // Tokenizar a entrada
//         tokenizer = tokenization_loop(input);
//         if (!tokenizer) {
//             printf("Erro ao tokenizar a entrada.\n");
//             free(input);
//             continue;
//         }

//         // Parsear os tokens em uma árvore de comandos
//         cmd_tree = parse_input(tokenizer->tokens);
//         if (!cmd_tree) {
//             printf("Erro ao parsear os tokens.\n");
//             free_all_tokens(tokenizer);
//             free(tokenizer->input);
//             free(tokenizer);
//             free(input);
//             continue;
//         }

//         // Imprimir a árvore de comandos (para teste)
//         printf("\nÁrvore de Comandos:\n");
//         print_cmd_tree(cmd_tree->root, 0);

//         // Liberar memória
//         free_all_tokens(tokenizer);
//         free(tokenizer->input);
//         free(tokenizer);
//         free_cmd_tree(cmd_tree);
//         free(input);
//     }

//     return 0;
// }

//################################# TOKENIZAÇÂO #############################//////
//  #include "../include/minishell.h"
// #include <stdio.h>
// #include <readline/readline.h>
// #include <readline/history.h>

// // Função para imprimir os tokens
// void print_tokens(t_token *tokens) {
//     while (tokens) {
//         printf("Token: [%s] | Tipo: ", tokens->value);
//         switch (tokens->type) {
//             case TOKEN_WORD:      printf("TOKEN_WORD\n"); break;
//             case TOKEN_SPACE:     printf("TOKEN_SPACE\n"); break;
//             case TOKEN_PIPE:      printf("TOKEN_PIPE\n"); break;
//             case TOKEN_REDIR_IN:  printf("TOKEN_REDIR_IN\n"); break;
//             case TOKEN_REDIR_OUT: printf("TOKEN_REDIR_OUT\n"); break;
//             case TOKEN_APPEND:    printf("TOKEN_APPEND\n"); break;
//             case TOKEN_HEREDOC:   printf("TOKEN_HEREDOC\n"); break;
//             case TOKEN_QUOTE:     printf("TOKEN_QUOTE\n"); break;
//             case TOKEN_DQUOTE:    printf("TOKEN_DQUOTE\n"); break;
//             case TOKEN_EOF:       printf("TOKEN_EOF\n"); break;
//             default:              printf("TIPO DESCONHECIDO\n"); break;
//         }
//         tokens = tokens->next;
//     }
// }

// int main(void) {
//     char *input;
//     t_tokenizer *tokenizer;

//     while (1) {
//         // Ler entrada do usuário
//        
//         if (!input) {
//             printf("exit\n");
//             break;
//         }

//         // Adicionar entrada ao histórico (se não estiver vazia)
//         if (*input)
//             add_history(input);

//         // Tokenizar a entrada
//         tokenizer = tokenization_loop(input);
//         if (!tokenizer) {
//             printf("Erro ao tokenizar a entrada.\n");
//             free(input);
//             continue;
//         }

//         // Imprimir os tokens gerados
//         printf("\nTokens gerados:\n");
//         print_tokens(tokenizer->tokens);

//         // Liberar memória
//         free_all_tokens(tokenizer);
//         free(tokenizer->input);
//         free(tokenizer);
//         free(input);
//     }

//     return 0;
// }

#include "../include/minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("Token: [%s] | Tipo: ", tokens->value);
        switch (tokens->type)
        {
            case TOKEN_WORD:      printf("TOKEN_WORD\n"); break;
            case TOKEN_PIPE:      printf("TOKEN_PIPE\n"); break;
            case TOKEN_REDIR_IN:  printf("TOKEN_REDIR_IN\n"); break;
            case TOKEN_REDIR_OUT: printf("TOKEN_REDIR_OUT\n"); break;
            case TOKEN_APPEND:    printf("TOKEN_APPEND\n"); break;
            case TOKEN_HEREDOC:   printf("TOKEN_HEREDOC\n"); break;
            case TOKEN_HEREDOC_DELIM:   printf("TOKEN_HEREDOC_DELIM\n"); break;  // Novo tipo para o delimitador
            case TOKEN_EOF:       printf("TOKEN_EOF\n"); break;
            default:              printf("TIPO DESCONHECIDO\n"); break;
        }
        tokens = tokens->next;
    }
}

void print_parsed_commands(t_cmd_list *cmd_list)
{
    int i;
    if (!cmd_list || !cmd_list->head)
    {
        printf("Nenhum comando encontrado.\n");
        return;
    }
    t_cmd_node *cmd = cmd_list->head;
    while (cmd)
    {
        printf("Command:\n");
        printf("  Arguments:");
        for (i = 0; cmd->args && cmd->args[i]; i++)
            printf(" [%s]", cmd->args[i]);
        printf("\n");
        if (cmd->in_fd > 0)
            printf("  Input Redirection: FD %d\n", cmd->in_fd);
        if (cmd->out_fd > 0)
            printf("  Output Redirection: FD %d\n", cmd->out_fd);
        if (cmd->next)
            printf("  |\n");
        cmd = cmd->next;
    }
    printf("---\n");
}

int main(void)
{
    char        *input;
    t_token     *tokens;
    t_cmd_list  *cmd_list;
    
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break;
        if (*input)
            add_history(input);
        
        tokens = tokenize_input(input);
        printf("\nTokens gerados:\n");
        print_tokens(tokens);
        
        cmd_list = parse_tokens(tokens);
        printf("\nComandos parseados:\n");
        print_parsed_commands(cmd_list);
        
        free(input);
        free_tokens(tokens);
        free_cmd_list(cmd_list);
    }
    return (0);
}

