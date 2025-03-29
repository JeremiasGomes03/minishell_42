NAME = minishell

SRCS = src/main.c \
		src/main_utils.c \
		src/parse_tokens.c \
		src/parse_command.c \
		src/parse_handlers.c \
		src/parse_utils.c \
		src/token.c \
		src/free.c \
		src/token_handlers.c \
		src/token_utils.c \
		src/builtins.c \
		src/exec.c \
		src/exec_utils.c \
		src/exec_utils2.c \
		src/utils.c \
		src/signal.c \
		src/expand_utils.c \
		src/expand_variable.c \
		src/expand_variable_utils.c \
		src/builtins_two.c \
		src/test_builtins.c \
		src/envp.c \
		src/process_heredoc_utils.c \
		src/process_heredoc.c \
		src/utils_builtins.c \

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror

LIBFT_DIR = lib/libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft -lreadline

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS)
	@$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
