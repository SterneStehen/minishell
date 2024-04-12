# Program name
NAME = minishell

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror

# Source directory and source files
SRC_DIR = src/
SRC_FILES = main.c \
            execute_command.c \
            init_shell.c \
            parser.c \
            util.c \
            builtins.c \
			#shell_loop.c \

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))

# Object directory and object files
OBJ_DIR = obj/
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

LIBFT = libft/libft.a
LIBFT_PATH = ./libft
LFLAGS = -L$(LIBFT_PATH) -lft -lreadline


# Include paths

INCLUDES = -I includes -I ./libft

# Default target
all: $(NAME)

# Linking the program
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LFLAGS) -o $(NAME)

# Compiling source files
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Adding a rule for making libft
$(LIBFT):
	make -C $(LIBFT_PATH)

# Removing object files
clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_PATH) clean

# Removing object files and the program binary
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean

# Re-making everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re
