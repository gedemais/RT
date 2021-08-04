include scripts/srcs.mk
include scripts/incs.mk

NAME=rt

CC=gcc
FLAGS= -Wall -Werror -Wextra

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
#
ifeq ($(OS), Darwin) # MacOs
FLAGS+=-DMACOS
MLX_PATH=libs/mlx_opengl
MLX=$(MLX_PATH)/libmlx.a
MLX_FLAGS= -Ilibs/mlx_opengl
MLX_LINKERS=$(MLX) -lm -framework OpenGL -framework AppKit
endif

ifeq ($(OS), Linux) # Linux
FLAGS+=-DLINUX
MLX_PATH=libs/mlx_linux
MLX=$(MLX_PATH)/libmlx.a
MLX_FLAGS= -Ilibs/mlx_linux
MLX_LINKERS=$(MLX) -L/usr/X11/include/ -lXext -lX11 -lm
endif

MLX=$(MLX_PATH)/libmlx.a

LIBFT_PATH=libs/libft
LIBFT=$(LIBFT_PATH)/libft.a

all: $(LIBFT) $(MLX) $(NAME)

$(NAME): $(OBJ)
	@echo "Linking $(NAME)..."
	$(CC) $(FLAGS) -I$(INC_PATH) -I$(LIBFT_PATH) -o $(NAME) $(OBJ) $(MLX_LINKERS) -framework OpenCL $(LIBFT)

$(SRC_PATH)%.o: $(SRC_PATH)%.c $(INC)
	@tput civis
	@printf " Compiling $<"
	@printf "                                       \\r"
	@tput cnorm
	@$(CC) $(FLAGS) -I$(INC_PATH) -I$(LIBFT_PATH) $(MLX_FLAGS) -o $@ -c $<

$(LIBFT): $(LIBFT_PATH)
	make -C $(LIBFT_PATH)

$(MLX): $(MLX_PATH)
	make -C $(MLX_PATH)

clean:
	@make -C $(MLX_PATH) clean
	@make -C $(LIBFT_PATH) clean
	@rm -rf $(OBJ)

fclean: clean
	@make -C $(LIBFT_PATH) fclean
	@rm -rf $(NAME)
	@rm -rf $(NAME).dSYM

re: fclean all
