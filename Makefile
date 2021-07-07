include scripts/srcs.mk
include scripts/incs.mk

NAME=rt

CC=gcc
FLAGS= -Wall -Werror -Wextra -Weverything

# Flags variables
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    FLAGS += -g3 -fsanitize=address
endif

OPTI ?= 0
ifeq ($(OPTI), 1)
    FLAGS += -Ofast -march=native
endif

SRC_PATH=src/
SRC=$(addprefix $(SRC_PATH), $(SRC_NAME))

INC_PATH=include/
INC=$(addprefix $(INC_PATH), $(INC_NAME))

OBJ=$(SRC:.c=.o)

OS = $(shell uname)

# Libs
ifeq ($(OS), Darwin) # MacOs
MLX_PATH=lib/mlx_opengl/
endif

ifeq ($(OS), Linux) # Linux
MLX_PATH=lib/mlx_linux/
endif

MLX=$(MLX_PATH)libmlx.a

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Linking $(NAME)..."
	$(CC) $(FLAGS) -I$(INC_PATH) -o $(NAME) $(OBJ) 

$(SRC_PATH)%.o: $(SRC_PATH)%.c $(INC)
	@tput civis
	@printf " Compiling $<"
	@printf "                                       \\r"
	@tput cnorm
	@$(CC) $(FLAGS) -I$(INC_PATH) -o $@ -c $<

clean:
	make -C $(MLX_PATH) clean
	@rm -rf $(OBJ)
	@echo "Libft clean"

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(NAME).dSYM

re: fclean all
