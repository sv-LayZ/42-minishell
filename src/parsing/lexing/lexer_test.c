#include "../../../include/minishell.h"

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
