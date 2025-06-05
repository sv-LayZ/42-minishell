/*Parcourir la liste de tokens

À chaque TOKEN_PIPE, créer une nouvelle t_cmd

Remplir les champs :

args : tous les TOKEN_COMMAND et TOKEN_ARGUMENT

infile : fichier après < ou <<

outfile : fichier après > ou >>

append / heredoc selon les cas*/

#include "../../include/minishell.h"

t_cmd *new_command(void)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_output = 0;
    // cmd->heredoc = 0;
    cmd->next = NULL;
    return cmd;
}
int add_arg(t_cmd *cmd, char *arg)
{
    int i = 0;
    char **new_args;

    if (!cmd || !arg)
        return 0;
    while (cmd->args && cmd->args[i])
        i++;
    new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return 0;
    i = 0;
    while (cmd->args && cmd->args[i])
    {
        new_args[i] = cmd->args[i];
        i++;
    }
    new_args[i] = strdup(arg);
    new_args[i + 1] = NULL;
    free(cmd->args);
    cmd->args = new_args;
    return 1;
}
