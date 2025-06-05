
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