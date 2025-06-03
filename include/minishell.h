#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <dirent.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define ENV_SEPARATOR ':'
# define ENV_PATH "PATH"

# define ERROR_MEMORY "Memory allocation error"
# define ERROR_READING_DIR "Error reading directory"

typedef enum e_token_type
{
	TOKEN_PIPE,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_IN,
	TOKEN_ENV_VAR,
	TOKEN_OPTION,
} e_token_type;

typedef struct s_token
{
	char *value;
	e_token_type type;
} t_token;

// /* **********************************PARSING**************************************** */
char	*reader(void);
char	*get_executable_path(char *str);

#endif