#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
#include <string.h>
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


typedef struct s_cmd
{
    char *name;
    char **args;
    char *input_file;
    char *output_file;
    int append_output; // 0 for overwrite, 1 for append
    struct s_cmd *next; // Pointer to the next command in a pipeline
} t_cmd;

typedef enum e_token_type
{
    TOKEN_PIPE,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_IN,
    TOKEN_ENV_VAR,
    TOKEN_COMMAND,
    TOKEN_ARGUMENT,
    TOKEN_OTHER,
    TOKEN_REDIRECT_APPEND,
    TOKEN_HEREDOC,
} t_token_type;

typedef struct s_operator_map
{
    char *op_str;
    t_token_type type;
} t_operator_map;

typedef struct t_token
{
    t_token_type type;
    char *value;
    int quoted;
    struct t_token *next; // Pointer to the next token in the linked list
    struct t_token *prev; // Pointer to the previous token in the linked list
} t_token;

// /* **********************************PARSING**************************************** */
char	*reader(void);
char *expand_variables(char *line);
int history_process(char *line);
// /* **********************************UTILS**************************************** */


// /* **********************************LEXING**************************************** */
t_token *line_lexer(char *line);
char **ft_split_str(char const *s, char *delimiters);

#endif