NAME = cub3d

# Исходные файлы
SOURCES = srcs/parsing.c \
		srcs/utils.c \
		srcs/free.c \
		srcs/textures_and_colors.c \
		srcs/parsing_map.c \
		srcs/clean_file.c \
		srcs/main.c \
		raycast/draw.c \
		raycast/move.c \
		raycast/init.c \
		raycast/raycast.c \
		raycast/utils1.c

# Объектные файлы
OBJECTS = $(SOURCES:.c=.o)

# Компилятор
CC = cc

# Флаги компилятора
CFLAGS = -Wall -Werror -Wextra -I./mlx -I./libft -g3


# Линковочные флаги для MinilibX
LIBS = -L./minilibx-linux -lmlx -lXext -lX11 -lm -L./libft -lft

# Основная цель
all: $(NAME)

# Правило для создания исполняемого файла
$(NAME): $(OBJECTS)
	@echo "Linking $@"
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(NAME)

# Правило для создания объектного файла из исходного
%.o: %.c
	@echo "Compiling $< into $@"
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка объектных файлов
clean:
	@echo "Cleaning object files"
	rm -f $(OBJECTS)

# Очистка всех файлов, включая исполняемое
fclean: clean
	@echo "Cleaning executable"
	rm -f $(NAME)

# Полная пересборка
re: fclean all

# Указание, что цели не являются файлами
.PHONY: all clean fclean re
