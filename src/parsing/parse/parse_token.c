/*Parcourir la liste de tokens

À chaque TOKEN_PIPE, créer une nouvelle t_cmd

Remplir les champs :

args : tous les TOKEN_COMMAND et TOKEN_ARGUMENT

infile : fichier après < ou <<

outfile : fichier après > ou >>

append / heredoc selon les cas*/

#include "../../../include/minishell.h"

t_cmd *parse_tokens(t_token *tokens)
{
    t_cmd *head = NULL;
    t_cmd *current = NULL;

    while (tokens)
    {
        if (!current && !(current = new_command()))
            return NULL;
        if (tokens->type == TOKEN_PIPE)
        {
            append_command(&head, current);
            current = NULL;
        }
        else if (is_argument_type(tokens->type))
        {
            if (!add_arg(current, tokens->value))
                return NULL;
        }
        else if (is_redirection(tokens->type))
        {
            if (!tokens->next || tokens->next->type != TOKEN_FILE)
                return NULL; // erreur syntaxe
            if (tokens->type == TOKEN_REDIRECT_IN)
                current->input_file = strdup(tokens->next->value);
            else if (tokens->type == TOKEN_REDIRECT_OUT)
                current->output_file = strdup(tokens->next->value);
            else if (tokens->type == TOKEN_REDIRECT_APPEND)
            {
                current->output_file = strdup(tokens->next->value);
                current->append_output = 1;
            }
            else if (tokens->type == TOKEN_HEREDOC)
            {
                current->input_file = strdup(tokens->next->value);
                //current->heredoc = 1;
            }
            tokens = tokens->next;
        }
        tokens = tokens->next;
    }
    if (current)
        append_command(&head, current);
    return head;
}

