#include "../../../include/minishell.h"

t_cmd *new_command(void)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_output = 0;
    cmd->heredoc = 0;
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
void append_command(t_cmd **head, t_cmd *new_cmd)
{
    t_cmd *tmp;

    if (!head || !new_cmd)
        return;
    if (!*head)
    {
        *head = new_cmd;
        return;
    }
    tmp = *head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_cmd;
}

void free_commands(t_cmd *cmd);

int is_argument_type(t_token_type type)
{
    return (type == TOKEN_COMMAND 
        || type == TOKEN_ARGUMENT || type == TOKEN_ENV_VAR);
}
int is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIRECT_OUT 
        || type == TOKEN_REDIRECT_IN 
        || type == TOKEN_REDIRECT_APPEND 
        || type == TOKEN_HEREDOC);
}

