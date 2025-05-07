NAME = cube3d
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(MLX_DIR)
MLX_DIR = ./minilibx-linux
LDFLAGS = -L$(MLX_DIR) -lmlx_Linux -lXext -lX11 -lm
SRCS = main.c cube3d.c utils.c
OBJS = $(SRCS:.c=.o)
RM = rm -f
all : $(NAME)
$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
clean :
	$(RM) $(OBJS)
fclean : clean
	$(RM) $(NAME)
re: fclean all