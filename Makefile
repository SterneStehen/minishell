# Имя исполняемого файла
NAME = minishell

# Компилятор и флаги компиляции
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Директории
SRCDIR = src
INCDIR = include
LIBFTDIR = $(INCDIR)/libft
BUILDDIR = build

# Файлы исходного кода
SRC = $(SRCDIR)/executor.c \
      $(SRCDIR)/init.c \
      $(SRCDIR)/lexer.c \
      $(SRCDIR)/loop.c \
      $(SRCDIR)/main.c \
      $(SRCDIR)/parser.c \
      $(SRCDIR)/signal.c

# Объектные файлы
OBJ = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Заголовочные файлы
INC = -I$(INCDIR) -I$(LIBFTDIR)

# Библиотеки
LIBFT = $(LIBFTDIR)/libft.a

# Цель по умолчанию
all: $(NAME)

# Сборка исполняемого файла
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFTDIR) -lft -lreadline -o $(BUILDDIR)/$(NAME)

# Сборка объектных файлов
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Создание директории для сборки
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Сборка библиотеки libft
$(LIBFT):
	make -C $(LIBFTDIR)

# Очистка объектных файлов и исполняемого файла
clean:
	rm -rf $(BUILDDIR)
	make -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(BUILDDIR)/$(NAME)
	make -C $(LIBFTDIR) fclean

# Пересборка
re: fclean all

# Не является файлом
.PHONY: all clean fclean re
