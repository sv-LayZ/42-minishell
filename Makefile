C = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRC = src/parsing/lexing/lexer.c 
HEADER = include/minishell.h

OBJ = $(SRC:.c=.o)
NAME = lexer_test

all: $(NAME)

LIB_DIR = libft
LIBFT = $(LIB_DIR)/libft.a


$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIB_DIR) -lft -lreadline -o $(NAME)

$(LIBFT): force
	$(MAKE) -C $(LIB_DIR) bonus
	
force:

%.o: %.c $(HEADER) Makefile
	$(CC) $(CFLAGS) -O3 -c $< -o $@

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIB_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIB_DIR) fclean

re: fclean all

.PHONY: all clean fclean re