/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lavinia <lavinia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:29 by jerda-si          #+#    #+#             */
/*   Updated: 2025/03/28 20:07:18 by lavinia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

static void	check_and_execute_exit(t_cmd_list *cmd_list)
{
	if (!cmd_list || !cmd_list->head || !cmd_list->head->args
		|| !cmd_list->head->args[0])
		return ;
	if (ft_strcmp(cmd_list->head->args[0], "exit") == 0)
		builtin_exit(cmd_list->head);
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
    len = ft_strlen(input);
    while (len > 0 && ft_isspace((unsigned char)input[len - 1]))
        len--;
    return (len > 0 && input[len - 1] == '|');
}

static int	check_syntax_errors(char *input)
{
	char	*trimmed;
	t_shell	shell;

	trimmed = trim_whitespace(input);
	if (ft_strcmp(trimmed, "$PATH") == 0)
	{
		printf("%s\n", get_envp(&shell, "PATH"));
		free(input);
		return (1);
	}
	if (ends_with_pipe(trimmed))
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		free(input);
		return (1);
	}
	return (0);
}

static int	validate_input(char *input)
{
	if (ft_strnstr(input, "&&", ft_strlen(input))
		|| ft_strnstr(input, "||", ft_strlen(input))
		|| ft_strnstr(input, "\\", ft_strlen(input)))
	{
		write(2,
			"minishell: unexpected token `&&' or `||' or `\\'\n", 49);
		free(input);
		return (0);
	}
	return (1);
}

static void process_command(char *input, t_shell *shell)
{
    t_token *tokens;

    tokens = tokenize_input(input);
    if (!validate_syntax(tokens))
    {
        free_tokens(tokens);
        free(input);
        return;
    }

    expander(&tokens, shell);
    shell->cmd_list = parse_tokens(tokens, shell);
    if (!shell->cmd_list)
    {
        free(input);
        return;
    }

    // Acessa o primeiro nó da lista de comandos
    t_cmd_node *cmd_node = shell->cmd_list->head;

    // Verifica se o comando é uma built-in
    if (is_builtin(cmd_node))  // Passando o nó correto
    {
        execute_builtin(cmd_node, shell);  // Executa a built-in com o nó correto
    }
    else
    {
        check_and_execute_exit(shell->cmd_list);  // Verifica e executa o exit se necessário
        execute_pipeline(shell->cmd_list, shell); // Executa os comandos no pipeline
    }

    free_cmd_list(shell->cmd_list);
    shell->cmd_list = NULL;
    free(input);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	shell;

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
			break ;
		if (*input)
			add_history(input);
		if (check_syntax_errors(input) || !validate_input(input))
			continue ;
		process_command(input, &shell);
	}
	free_envp(shell.envp);
	return (0);
}
