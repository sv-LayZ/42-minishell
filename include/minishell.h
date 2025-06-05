#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <dirent.h>
# include <errno.h>
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
    TOKEN_COMMAND,
    TOKEN_ARGUMENT,
    TOKEN_OTHER,
} t_token_type;

typedef struct t_token
{
    t_token_type type;
    char **value;
} t_token;

// /* **********************************PARSING**************************************** */
char	*reader(void);
char *expand_variables(char *line);
int history_process(char *line);
// /* **********************************UTILS**************************************** */

char **ft_split_str(char const *s, char *delimiters);

#endif