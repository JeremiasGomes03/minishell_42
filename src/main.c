/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:29 by jerda-si          #+#    #+#             */
/*   Updated: 2025/02/27 22:36:13 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>


static void process_input(char *input, t_token **tokens, t_cmd_list **cmd_list)
{
    *tokens = tokenize_input(input);
    if (!validate_syntax(*tokens))
    {
        printf("Erro de sintaxe\n");
        free(input);
        free_tokens(*tokens);
        *tokens = NULL;
        return;
    }
    *cmd_list = parse_tokens(*tokens);
}

static void check_and_execute_exit(t_cmd_list *cmd_list)
{
    if (cmd_list->head && cmd_list->head->args && ft_strcmp(cmd_list->head->args[0], "exit") == 0)
    {
        builtin_exit(cmd_list->head);
    }
}

static void execute_and_cleanup(t_cmd_list *cmd_list, char *input, t_token *tokens)
{
    if (cmd_list)
    {
        execute_pipeline(cmd_list);
        free_cmd_list(cmd_list);
    }
    free(input);
    free_tokens(tokens);
}

int main(void)
{
    char        *input;
    t_token     *tokens;
    t_cmd_list  *cmd_list;

    setup_signals();
    while (1)
    {
        input = readline("minishell> ");
        if (!input) break;
        if (*input) add_history(input);
        process_input(input, &tokens, &cmd_list);
        if (cmd_list)
            check_and_execute_exit(cmd_list);
        execute_and_cleanup(cmd_list, input, tokens);
    }
    return 0;
}

void builtin_exit(t_cmd_node *cmd)
{
    int exit_code = 0;

    if (cmd->args[1])
    {
        exit_code = ft_atoi(cmd->args[1]);
        if (!ft_isnumeric(cmd->args[1]))
        {
            printf("exit: %s: numeric argument required\n", cmd->args[1]);
            exit_code = 255;
        }
    }
    printf("exit\n");
    exit(exit_code);
}

