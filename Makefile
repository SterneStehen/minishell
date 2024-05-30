NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c lexer.c parser.c executor.c builtins.c utils.c
OBJS = $(SRCS:.c=.o)
LIBFT = ./include/libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
    $(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
    $(MAKE) -C ./libft

clean:
    rm -f $(OBJS)
    $(MAKE) -C ./libft clean

fclean: clean
    rm -f $(NAME)
    $(MAKE) -C ./libft fclean

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus
