NAME = minishell

SRCS = src/main.c \
		src/parse.c \
		src/parse_handlers.c \
		src/parse_utils.c \
		src/token.c \
		src/token_handlers.c \
		src/token_utils.c \
		src/builtins.c \
		src/exec.c \
		src/utils.c \
		src/signal.c \
		src/expansion.c \

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
