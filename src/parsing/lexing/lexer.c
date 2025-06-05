#include "../../../include/minishell.h"

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
int total_len = token_len(&line[i], &quoted);
int val_len = total_len;
        int start = i;
        if (quoted && (line[i] == '\'' || line[i] == '\"'))
        {
            start++;
            val_len -= 2;
        }

        char *val = ft_strndup(&line[start], val_len);
        t_token_type type = detect_type(&line[i], quoted, last_type);

        t_token *tok = create_token(type, val, quoted);
        add_token(&head, &tail, tok);

            free(val);
            if (quoted)
                i += total_len + 2;
            else
                i += total_len;  

        last_type = type;
    }
    return head;
}
