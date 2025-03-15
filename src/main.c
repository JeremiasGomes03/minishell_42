/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:29 by jerda-si          #+#    #+#             */
/*   Updated: 2025/03/15 20:17:52 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>


static void process_input(char *input, t_token **tokens, t_cmd_list **cmd_list, t_shell *shell)
{
    *tokens = tokenize_input(input);
    if (!validate_syntax(*tokens))
    {
        printf("Erro de sintaxe\n");
        free_tokens(*tokens);
        *tokens = NULL;
        *cmd_list = NULL;   
        return;
    }
    t_token *first_token = *tokens;
    if (first_token && !first_token->next && first_token->value[0] == '$')
    {
        char *var_value = get_envp(shell, first_token->value + 1);
        if (var_value)
        {
            printf("%s\n", var_value);
            free(var_value);
        }
        free_tokens(*tokens);
        *tokens = NULL;
        *cmd_list = NULL;
        return;
    }
    expander(tokens, shell);
    *cmd_list = parse_tokens(shell->tokens, shell);
}

static void check_and_execute_exit(t_cmd_list *cmd_list)
{
    if (cmd_list->head && cmd_list->head->args && ft_strcmp(cmd_list->head->args[0], "exit") == 0)
    {
        builtin_exit(cmd_list->head);
    }
}

static void execute_and_cleanup(t_cmd_list **cmd_list, char **input, t_token **tokens, t_shell *shell)
{
    if (*cmd_list)
    {
        execute_pipeline(*cmd_list, shell);
        free_cmd_list(*cmd_list);
        *cmd_list = NULL;
    }
    if (*input)
    {
        free(*input);
        *input = NULL;
    }
    if (*tokens)
    {
        free_tokens(*tokens);
        *tokens = NULL;
    }
}

char *trim_whitespace(char *str)
{
    char *end;

    if (str == NULL || *str == '\0')
        return str;
    while (ft_isspace((unsigned char)*str))
        str++;
    if (*str == '\0')
        return str;
    end = str + ft_strlen(str) - 1;
    while (end > str && ft_isspace((unsigned char)*end))
        end--;
    *(end + 1) = '\0';

    return str;
}

int main(int argc, char **argv, char **envp)
{
    char *input;
    t_shell shell;

    (void)argc;
    (void)argv;
     
    shell.envp = dup_envp(envp);
    shell.exit_status = 0;
    shell.cmd_list = NULL;
    shell.tokens = NULL;
    setup_signals();

    while (1)
    {
        input = readline("\x1b[1m\x1b[92mminishell\x1b[37m>\x1b[0m ");
        if (!input)
            break;
        if (*input)
            add_history(input);
        char *trimmed = trim_whitespace(input);
        if (ft_strcmp(trimmed, "$PATH") == 0)
        {
            char *path = get_envp(&shell, "PATH");
            if (path)
            {
                printf("%s\n", path);
                free(path);
            }
            free(input);
            continue;
        }
        process_input(input, &shell.tokens, &shell.cmd_list, &shell);
        if (shell.cmd_list)
        {
            check_and_execute_exit(shell.cmd_list);
            execute_and_cleanup(&shell.cmd_list, &input, &shell.tokens, &shell);
        }
        else
        {
            if (input)
                free(input);
            if (shell.tokens)
                free_tokens(shell.tokens);
        }
    }
    free_envp(shell.envp);
    return (0);
}

