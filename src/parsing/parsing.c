

/*La fonction parse_tokens transforme la liste de tokens (issue du lexer) en une liste chaînée de structures t_cmd, chacune représentant une commande à exécuter, avec ses arguments, ses fichiers de redirection, et les informations de pipe/heredoc.

Cela aide l'exécuteur car il n'a plus à analyser la ligne brute : il reçoit une structure claire et prête à l’emploi, où :

Chaque nœud t_cmd correspond à une commande du pipeline.
Les champs args, input_file, output_file, append_output, heredoc sont déjà remplis.
La liste chaînée (next) permet de parcourir les commandes à exécuter dans l’ordre (utile pour gérer les pipes).*/

#include "../../include/minishell.h"

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
            if(tokens->quote_type == DOUBLE_QUOTE)
            {
                char *expanded = expand_variables(tokens->value);
                free(tokens->value);
                tokens->value = expanded;
                if (!tokens->value)
                    return NULL; // erreur d'expansion
            }
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
                current->heredoc = 1;
            }
            tokens = tokens->next;
        }
        tokens = tokens->next;
    }
    if (current)
        append_command(&head, current);
    return head;
}

t_cmd *parsing(const char *line)
{
    t_token *tokens;
    t_cmd   *cmds;

    tokens = line_lexer(line);
    if (!tokens)
        return NULL;
    cmds = parse_tokens(tokens);
    free_tokens(tokens);
    return cmds;
}