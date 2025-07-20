C = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin) #macOS
	CFLAGS += -I/opt/homebrew/opt/readline/include
	LDFLAGS = -L/opt/homebrew/opt/readline/lib
endif

SRC = src/parsing/lexing/lexer.c \
	src/parsing/lexing/lexer_utils.c \
	src/parsing/lexing/detect_type.c \
	src/parsing/parsing.c \
	src/parsing/parse/parse_utils.c \
	src/parsing/parse/parse_test.c \
	src/parsing/expand_variables.c \
	src/parsing/reader.c \
	src/signals/signals.c \
	src/execution.c \
	src/main.c \
	src/builtins/builtin_utils.c \
	src/builtins/builtin_echo.c \
	src/builtins/builtin_cd.c \
	src/builtins/builtin_pwd.c \
	src/builtins/builtin_export.c \
	src/builtins/builtin_unset.c \
	src/builtins/builtin_env.c \
	src/builtins/builtin_exit.c
	
HEADER = include/minishell.h

OBJ = $(SRC:.c=.o)
NAME = test_shell

all: $(NAME)

LIB_DIR = libft
LIBFT = $(LIB_DIR)/libft.a

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIB_DIR) -lft $(LDFLAGS) -lreadline -o $(NAME)

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