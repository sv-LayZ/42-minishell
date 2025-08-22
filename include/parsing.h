#ifndef PARSING_H
#define PARSING_H

typedef enum e_redir_type
{
    R_IN,
    R_OUT_TRUNC,
    R_OUT_APPEND,
    R_HEREDOC
}   t_redir_type;

typedef struct s_redir
{
    t_redir_type        type;
    char                *filename; // or limiter when HEREDOC
    struct s_redir      *next;
}   t_redir;

typedef struct s_cmd
{
    char        *name;
    char        **args;
    int         heredoc; // 0 for no heredoc, 1 for heredoc
    char        *input_file; // legacy single tracking (to be removed after migration)
    char        *output_file; // legacy
    int         append_output; // legacy
    t_redir     *redirs; // ordered list of redirections as they appeared
    struct s_cmd *next; // Pointer to the next command in a pipeline
} t_cmd;

typedef struct s_quote_data
{
	size_t	*i;
	char	*res;
	size_t	*j;
	size_t	len;
}	t_quote_data;

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

typedef enum e_quote_type
{
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    NO_QUOTE
} t_quote_type;

typedef struct s_operator_map
{
    char *op_str;
    t_token_type type;
} t_operator_map;

typedef struct t_token
{
    t_token_type type;
    char *value;
    t_quote_type quote_type;
    struct t_token *next; // Pointer to the next token in the linked list
    struct t_token *prev; // Pointer to the previous token in the linked list
} t_token;

// /* **********************************PARSING**************************************** */
char	*reader(void);
int history_process(char *line);
char *expand_variables(const char *line);
t_quote_type	get_quote_type(const char *value);

// /* **********************************UTILS**************************************** */
void free_commands(t_cmd *cmd);
char *remove_quotes(const char *str);
void    free_redirs(t_redir *r);
// /* **********************************LEXING**************************************** */

t_token_type get_token_type(const char *value);
t_token *create_token(t_token_type type, const char *value, int quoted);
void append_token(t_token **head, t_token *new_tok);
int operator_token_length(const char *str);
char *extract_token(const char *input, int *index);
char *extract_quoted_token(const char *input, int *i);
char *extract_operator_token(const char *input, int *i);
char *extract_word_token(const char *input, int *i);
int is_operator_char(char c);
t_token *new_token(const char *value, t_quote_type quoted);
t_token_type get_redirection_type(const char *line);
t_token_type get_operator_type(const char *line);
void free_tokens(t_token *head);

// /* **********************************PARSE**************************************** */
int is_argument_type(t_token_type type);
int is_redirection(t_token_type type);
int add_arg(t_cmd *cmd, char *arg);
void append_command(t_cmd **head, t_cmd *new_cmd);
t_cmd *new_command(void);
t_cmd *parsing(const char *line);
t_cmd *parse_tokens(t_token *tokens);
t_token *line_lexer(const char *line);
int handle_redirection(t_cmd *current, t_token *tokens);
#endif
