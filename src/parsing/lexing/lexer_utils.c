#include "../../../include/minishell.h"

t_token *new_token(const char *value, t_quote_type quoted)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->value = strdup(value);
    token->quote_type = quoted;
    token->type = get_token_type(value);
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


void append_token(t_token **head, t_token *new_tok)
{
    if (!*head)
    {
        *head = new_tok;
        return;
    }
    t_token *temp = *head;
    while (temp->next)
        temp = temp->next;
    temp->next = new_tok;
    new_tok->prev = temp;
}

int is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

// Retourne la longueur du token opérateur (1 ou 2 pour >>, <<)
int operator_token_length(const char *str)
{
    if ((str[0] == '>' && str[1] == '>') || (str[0] == '<' && str[1] == '<'))
        return 2;
    return 1;
}

// Supprime les guillemets simples ou doubles entourant une chaîne, si présents
char *remove_quotes(const char *str)
{
    size_t len;
    char *res;

    if (!str)
        return NULL;
    len = strlen(str);
    if (len >= 2 && ((str[0] == '"' && str[len-1] == '"') || (str[0] == '\'' && str[len-1] == '\'')))
    {
        res = malloc(len - 1); // len - 2 (quotes) + 1 (null)
        if (!res)
            return NULL;
        strncpy(res, str + 1, len - 2);
        res[len - 2] = '\0';
        return res;
    }
    return strdup(str);
}

// int main()
// {
//     const char *input = "echo \"hello $USER\" | grep 'abc $USER' > output.txt";
//     int index = 0;
//     char *token;

//     while ((token = extract_token(input, &index)) != NULL)
//     {
//         printf("Token: [%s]\n", token);
//         free(token);
//     }

//     return 0;
// }
