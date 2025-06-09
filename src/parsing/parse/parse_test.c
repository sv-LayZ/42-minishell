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
