#include "../../include/minishell.h"

t_cmd *new_command(void);

void add_arg(t_cmd *cmd, char *arg);

void append_command(t_cmd **list, t_cmd *cmd);
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

int is_argument_type(t_token_type type);

int is_redirection(t_token_type type);

