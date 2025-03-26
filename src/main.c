/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeremias <jeremias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:29 by jerda-si          #+#    #+#             */
/*   Updated: 2025/03/26 18:02:09 by jeremias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>


static void check_and_execute_exit(t_cmd_list *cmd_list)
{
    if (cmd_list->head && cmd_list->head->args && ft_strcmp(cmd_list->head->args[0], "exit") == 0)
    {
        builtin_exit(cmd_list->head);
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

int ends_with_pipe(const char *input)
{
    int len;

    if (!input)
        return 0;
    len = strlen(input);
    while (len > 0 && isspace((unsigned char)input[len - 1]))
        len--;
    return (len > 0 && input[len - 1] == '|');
}

int main(int argc, char **argv, char **envp)
{
    char        *input;
    t_shell     shell;
    t_token     *tokens;
    t_token     *original_tokens;
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
        if (ends_with_pipe(trimmed))
        {
            write(2, "minishell: syntax error near unexpected token `|'\n",  51);

            free(input);
            continue;
        }
        if (ft_strnstr(input, "&&", ft_strlen(input)) 
            || ft_strnstr(input, "||", ft_strlen(input)) 
                || ft_strnstr(input, "\\", ft_strlen(input)))
        {
            write(2, "minishell: syntax error near unexpected token `&&' or `||' or `\\'\n", 67);
            free(input);
            continue;
        }
        tokens = tokenize_input(input);
        original_tokens = tokens;
        if (!validate_syntax(tokens))
        {
            free_tokens(original_tokens);
            free(input);
            continue;
        }
        expander(&tokens, &shell);
        shell.cmd_list = parse_tokens(original_tokens, &shell);
        if (!shell.cmd_list)
        {
            close(STDIN_FILENO);
            int new_stdin = open("/dev/tty", O_RDONLY);
            if (new_stdin != -1)
            {
                dup2(new_stdin, STDIN_FILENO);
                close(new_stdin);
            }
            free(input);
            continue;
        }
        check_and_execute_exit(shell.cmd_list);
        execute_pipeline(shell.cmd_list, &shell);
        free_cmd_list(shell.cmd_list);
        shell.cmd_list = NULL;
        free(input);
    }
    free_envp(shell.envp);
    return (0);
}


