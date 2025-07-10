#include "../../../include/minishell.h"


static void skip_whitespace(const char *input, int *i)
{
    while (ft_isspace(input[*i]))
        (*i)++;
}

static char *extract_quoted_token(const char *input, int *i)
{
    int start = *i;
    char quote = input[(*i)++];
    while (input[*i] && input[*i] != quote)
        (*i)++;
    if (input[*i] == quote)
        (*i)++;
    int len = *i - start;
    char *token = malloc(len + 1);
    if (!token)
        return NULL;
    strncpy(token, &input[start], len);
    token[len] = '\0';
    return token;
}

static char *extract_operator_token(const char *input, int *i)
{
    int len = operator_token_length(&input[*i]);
    char *token = malloc(len + 1);
    if (!token)
        return NULL;
    strncpy(token, &input[*i], len);
    token[len] = '\0';
    *i += len;
    return token;
}

static char *extract_word_token(const char *input, int *i)
{
    int start = *i;
    while (input[*i] && !isspace(input[*i]) && !is_operator_char(input[*i]))
        (*i)++;
    int len = *i - start;
    char *token = malloc(len + 1);
    if (!token)
        return NULL;
    strncpy(token, &input[start], len);
    token[len] = '\0';
    return token;
}

char *extract_token(const char *input, int *index)
{
    int i = *index;

    skip_whitespace(input, &i);
    if (input[i] == '\0')
        return NULL;

    char *token;
    if (input[i] == '\'' || input[i] == '"')
        token = extract_quoted_token(input, &i);
    else if (is_operator_char(input[i]))
        token = extract_operator_token(input, &i);
    else
        token = extract_word_token(input, &i);

    *index = i;
    return token;
}

t_token_type get_token_type(const char *value)
{
    if (!value)
        return TOKEN_OTHER;

    if (strcmp(value, "|") == 0)
        return TOKEN_PIPE;
    else if (strcmp(value, ">") == 0)
        return TOKEN_REDIRECT_OUT;
    else if (strcmp(value, "<") == 0)
        return TOKEN_REDIRECT_IN;
    else if (strcmp(value, ">>") == 0)
        return TOKEN_REDIRECT_APPEND;
    else if (strcmp(value, "<<") == 0)
        return TOKEN_HEREDOC;
    else if (value[0] == '$')
        return TOKEN_ENV_VAR;

    return TOKEN_OTHER; // Par défaut
}
