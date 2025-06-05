#include "../../../include/minishell.h"




static int is_redirect_type(int type)
{
    return (type == TOKEN_REDIRECT_OUT ||
            type == TOKEN_REDIRECT_IN ||
            type == TOKEN_REDIRECT_APPEND ||
            type == TOKEN_HEREDOC);
}

void refine_token_types(t_token *head)
{
    t_token *tmp = head;
    int command_found = 0;

    while (tmp)
    {
         if (tmp->type == TOKEN_PIPE)
            command_found = 0;
        if (tmp->type == TOKEN_OTHER)
        {
            if (!command_found)
            {
                tmp->type = TOKEN_COMMAND;
                command_found = 1;
            }
            else if (tmp->prev && is_redirect_type(tmp->prev->type))
                tmp->type = TOKEN_FILE;
            else
                tmp->type = TOKEN_ARGUMENT;
        }
        tmp = tmp->next;
    }
}
// La fonction line_lexer complète
t_token *line_lexer(const char *line)
{
    int index = 0;
    char *value;
    t_token *head = NULL;
    t_token *token;

    while ((value = extract_token(line, &index)) != NULL)
    {
        int quoted = 0;
        size_t len = strlen(value);
        if ((value[0] == '\'' && value[len-1] == '\'')
        || (value[0] == '"' && value[len-1] == '"'))
            quoted = 1;
        token = new_token(value, quoted);
        free(value);
        if (!token)
        {
            free_tokens(head);
            return NULL;
        }
        append_token(&head, token);
    }
    refine_token_types(head);
    return head;
}



// // Test
// int main()
// {
//     const char *input = "ZAZA=ddd & echo \"hello $USER\" zizi zooz -e fdsfsf | grep 'abc $USER' > output.txt";
//     t_token *tokens = line_lexer(input);

//     t_token *tmp = tokens;
//     while (tmp)
//     {
//         printf("Token: [%s], Type: %d, Quoted: %d\n", tmp->value, tmp->type, tmp->quoted);
//         tmp = tmp->next;
//     }

//     free_tokens(tokens);
//     return 0;
// }
