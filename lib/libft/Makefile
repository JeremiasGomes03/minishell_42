NAME = libft.a

SRCS = 	ft_strlen.c \
		ft_isdigit.c \
		ft_isalpha.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_strncmp.c \
		ft_strcmp.c \
		ft_strlcat.c \
		ft_strdup.c \
		ft_isprint.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_strnstr.c \
		ft_strlcpy.c \
		ft_strchr.c \
		ft_strrchr.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memcmp.c \
		ft_memmove.c \
		ft_memchr.c \
		ft_atoi.c \
		ft_calloc.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_substr.c \
		ft_strtrim.c \
		ft_strmapi.c \
		ft_striteri.c \
		ft_strjoin.c \
		ft_itoa.c \
		ft_split.c \
		ft_strjoin_with_free.c \
		get_next_line/get_next_line.c \
		get_next_line/get_next_line_utils.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $@ $^
	@echo "Library $(NAME) created successfully!"

%.o: %.c

	@$(CC) $(CFLAGS) -c $< -o $@
new_fun: get_next_line/get_next_line.c get_next_line/get_next_line_utils.c
	$(CC) $(CFLAGS) -c $^
	@echo "Additional functions compiled successfully!"

clean:
	@rm -f $(OBJS)
	@echo "Objects cleaned!"

fclean: clean
	@rm -f $(NAME)
	@echo "Full clean completed!"

re: fclean all

.PHONY: all clean fclean re new_fun
