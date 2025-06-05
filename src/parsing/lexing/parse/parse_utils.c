t_command *new_command(void);

void add_arg(t_command *cmd, char *arg);

void append_command(t_command **list, t_command *cmd);
void append_command(t_command **head, t_command *new_cmd)
{
    t_command *tmp;

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

void free_commands(t_command *cmd);

int is_argument_type(t_token_type type);

int is_redirection(t_token_type type);

