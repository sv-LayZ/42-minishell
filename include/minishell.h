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
    TOKEN_REDIRECT_APPEND,
    TOKEN_HEREDOC,
    TOKEN_FILE,
    TOKEN_OTHER
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
int history_process(char *line);
// /* **********************************UTILS**************************************** */


// /* **********************************LEXING**************************************** */

t_token_type get_token_type(const char *value);
t_token *create_token(t_token_type type, const char *value, int quoted);
void append_token(t_token **head, t_token *new_tok);
int operator_token_length(const char *str);
char *extract_token(const char *input, int *index);
int is_operator_char(char c);
t_token *new_token(const char *value, int quoted);
t_token_type get_redirection_type(const char *line);
t_token_type get_operator_type(const char *line);
void free_tokens(t_token *head);

// /* **********************************PARSE**************************************** */
int is_argument_type(t_token_type type);
int is_redirection(t_token_type type);
char **ft_split_str(char const *s, char *delimiters);
int add_arg(t_cmd *cmd, char *arg);
void append_command(t_cmd **head, t_cmd *new_cmd);
t_cmd *new_command(void);
t_cmd *parse_tokens(t_token *tokens);
t_token *line_lexer(const char *line);
#endif