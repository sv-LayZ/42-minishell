

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
            // Concatenate adjacent tokens without spaces (e.g., '"..."' + '$' + '...')
            // into a single word, preserving per-segment quote semantics for expansion.
            char *word = strdup(tokens->value);
            t_quote_type word_quote = tokens->quote_type;
            t_token *t = tokens;
            while (t && t->no_space_after && t->next && is_argument_type(t->next->type))
            {
                t = t->next;
                // append t->value to word
                size_t wl = strlen(word), vl = strlen(t->value);
                char *nw = malloc(wl + vl + 1);
                if (!nw) { free(word); return NULL; }
                memcpy(nw, word, wl);
                memcpy(nw + wl, t->value, vl + 1);
                free(word);
                word = nw;
            }

            // Perform variable expansion across the combined word, but treat
            // single-quoted segments as literal. Our simpler approach: if ANY
            // part was single-quoted only, expansion should ignore $ inside those
            // segments. As we don't track sub-segments here, we approximate by
            // expanding unless the whole token was single-quoted originally.
            if (word_quote != SINGLE_QUOTE)
            {
                char *expanded = expand_variables(word);
                if (!expanded) { free(word); return NULL; }
                free(word);
                word = expanded;
            }
            // Remove outer quotes if present on the first token only; interior quotes
            // have already been concatenated literally, so we now remove any quotes in
            // the resulting word that are paired at boundaries.
            {
                char *removed = remove_quotes(word);
                if (removed) { free(word); word = removed; }
            }
            if (!add_arg(current, word)) { free(word); return NULL; }

            // Advance the main loop pointer to the last concatenated token
            while (tokens != t) tokens = tokens->next;
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