#include "../../../include/minishell.h"

t_token *create_token(t_token_type type, const char *value, int quoted)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
    {
        perror(ERROR_MEMORY);
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->value = ft_strdup(value);
    token->quoted = quoted;
    token->next = NULL;
    token->prev = NULL;
    return token;
}
void free_tokens(t_token *head)
{
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
}


void add_token(t_token **head, t_token **tail, t_token *new_token)
{
    if (!*head)
    {
        *head = new_token;
        *tail = new_token;
    }
    else
    {
        new_token->prev = *tail;
        (*tail)->next = new_token;
        *tail = new_token;
    }
}


int skip_whitespace(const char *line)
{
    int i = 0;
    while (line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;
    return i;
}
int is_quote(char c)
{
    return (c == '\'' || c == '\"');
}


t_token_type get_operator_type(const char *line)
{
    static const t_operator_map op_map[] = {
        {"|", TOKEN_PIPE},
        {"$", TOKEN_ENV_VAR},
        {NULL, TOKEN_OTHER} // Fin du tableau
    };

    int i = 0;
    while (op_map[i].op_str != NULL)
    {
        size_t len = ft_strlen(op_map[i].op_str);
        if (ft_strncmp(line, op_map[i].op_str, len) == 0)
            return op_map[i].type;
        i++;
    }
    return TOKEN_OTHER;
}

t_token_type get_redirection_type(const char *line)
{
    static const t_operator_map redir_map[] = {
        {">>", TOKEN_REDIRECT_APPEND},
        {"<<", TOKEN_HEREDOC},
        {">", TOKEN_REDIRECT_OUT},
        {"<", TOKEN_REDIRECT_IN},
        {NULL, TOKEN_OTHER}
    };

    int i = 0;
    while (redir_map[i].op_str != NULL)
    {
        size_t len = ft_strlen(redir_map[i].op_str);
        if (strncmp(line, redir_map[i].op_str, len) == 0)
            return redir_map[i].type;
        i++;
    }
    return TOKEN_OTHER;
}
t_token_type detect_type(const char *s, int quoted, t_token_type last_type)
{
    t_token_type type;

    if (!quoted)
    {
        type = get_redirection_type(s);
        if (type != TOKEN_OTHER)
            return type;

        type = get_operator_type(s);
        if (type != TOKEN_OTHER)
            return type;
    }

    // Déduction par contexte
    if (last_type == TOKEN_PIPE || last_type == TOKEN_OTHER)
        return TOKEN_COMMAND;

    if (last_type == TOKEN_REDIRECT_OUT || last_type == TOKEN_REDIRECT_APPEND ||
        last_type == TOKEN_REDIRECT_IN || last_type == TOKEN_HEREDOC)
        return TOKEN_ARGUMENT;

    return TOKEN_ARGUMENT;
}

int token_len(const char *line, int *quoted)
{
    int i = 0;
    *quoted = 0;

    if (line[i] == '\'' || line[i] == '\"')
    {
        char quote_char = line[i];
        *quoted = 1;
        i++; // Skip the opening quote

        while (line[i] && line[i] != quote_char)
            i++;

        if (line[i] == quote_char)
            i++; // Include the closing quote
        return i;
    }
    else
    {
        while (line[i] && line[i] != ' ' && line[i] != '\t' &&
               !is_quote(line[i]) &&
               get_redirection_type(&line[i]) == TOKEN_OTHER &&
               get_operator_type(&line[i]) == TOKEN_OTHER)
        {
            i++;
        }
        return i;
    }
}

/*Analyser la ligne de commande line,
découper cette ligne en tokens (unités lexicales) correctement typées,
et construire une liste chaînée de ces tokens.*/

t_token *line_lexer(char *line)
{
    t_token *head = NULL;
    t_token *tail = NULL;
    t_token_type last_type = TOKEN_OTHER;

    int i = 0;
    while (line[i])
    {
        i += skip_whitespace(&line[i]);
        if (!line[i])
            break;

        int quoted = 0;
        int len = token_len(&line[i], &quoted);

        int start = i;
        if (quoted && (line[i] == '\'' || line[i] == '\"'))
        {
            start++;
            len -= 2;
        }

        char *val = ft_strndup(&line[start], len);
        t_token_type type = detect_type(&line[i], quoted, last_type);

        t_token *tok = create_token(type, val, quoted);
        add_token(&head, &tail, tok);

            free(val);
            if (quoted)
                i += len + 2;
            else
                i += len;  

        last_type = type;
    }
    return head;
}
const char *token_type_to_str(t_token_type type)
{
    switch (type)
    {
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_REDIRECT_OUT: return "REDIRECT_OUT";
        case TOKEN_REDIRECT_IN: return "REDIRECT_IN";
        case TOKEN_ENV_VAR: return "ENV_VAR";
        case TOKEN_COMMAND: return "COMMAND";
        case TOKEN_ARGUMENT: return "ARGUMENT";
        case TOKEN_OTHER: return "OTHER";
        case TOKEN_REDIRECT_APPEND: return "REDIRECT_APPEND";
        case TOKEN_HEREDOC: return "HEREDOC";
        default: return "UNKNOWN";
    }
}

void print_tokens(t_token *head)
{
    t_token *cur = head;
    while (cur)
    {
        printf("Token: %-15s | Value: \"%s\" | Quoted: %d\n",
               token_type_to_str(cur->type), cur->value, cur->quoted);
        cur = cur->next;
    }
}

int main(void)
{
    char *input = "echo \"hello $USER\" | grep 'abc $USER' > output.txt";

    t_token *tokens = line_lexer(input);

    print_tokens(tokens);

free_tokens(tokens);
    return 0;
}
