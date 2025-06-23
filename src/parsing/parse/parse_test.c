#include "../../../include/minishell.h"

// Fonction utilitaire pour afficher la liste de commandes
void print_commands(t_cmd *cmd)
{
    int i;
    while (cmd)
    {
        printf("=== Command ===\n");
        if (cmd->args)
        {
            printf("Args: ");
            for (i = 0; cmd->args[i]; i++)
                printf("[%s] ", cmd->args[i]);
            printf("\n");
        }
        if (cmd->input_file)
            printf("Input: %s\n", cmd->input_file);
        if (cmd->output_file)
            printf("Output: %s (append: %d)\n", cmd->output_file, cmd->append_output);
        printf("\n");
        cmd = cmd->next;
    }
}

// int main(int ac, char **av)
// {
//     if(ac == 2)
//     {
//         const char *input = av[1];
//         t_token *tokens = line_lexer(input);
//         free_tokens(tokens); // à implémenter si besoin

//         printf("=== Tokens ===\n");
//         t_token *tmp = tokens;
//         while (tmp)
//         {
//             printf("Type: %d, Value: %s\n", tmp->type, tmp->value);
//             tmp = tmp->next;
//         }
//         t_cmd *cmds = parse_tokens(tokens);
//         printf("\n=== Parsed Commands ===\n");
//         print_commands(cmds);
//         free_commands(cmds); // à implémenter si besoin
//     }
//     return 0;
// }