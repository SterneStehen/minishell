NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src/
SRC_FILES = main.c \
            execute_command.c \
            init_shell.c \
            parser.c \
            util.c \
            handle.c \
			#shell_loop.c \

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_DIR = obj/
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

LIBFT = /inc/libft/libft.a
LIBFT_PATH = ./inc/libft
LFLAGS = -L$(LIBFT_PATH) -lft -lreadline

INCLUDES = -I ./inc/libft

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LFLAGS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_PATH)

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_PATH) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re
