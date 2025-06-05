#include "../../../include/minishell.h"


t_token_type get_operator_type(const char *line)
{
    static const t_operator_map op_map[] = {
        {"|", TOKEN_PIPE},
        {"$", TOKEN_ENV_VAR},
        {NULL, TOKEN_OTHER} // Fin du tableau
    };

    int i = 0;
    while (op_map[i].op_str != NULL)
    {
        size_t len = ft_strlen(op_map[i].op_str);
        if (ft_strncmp(line, op_map[i].op_str, len) == 0)
            return op_map[i].type;
        i++;
    }
    return TOKEN_OTHER;
}

t_token_type get_redirection_type(const char *line)
{
    static const t_operator_map redir_map[] = {
        {">>", TOKEN_REDIRECT_APPEND},
        {"<<", TOKEN_HEREDOC},
        {">", TOKEN_REDIRECT_OUT},
        {"<", TOKEN_REDIRECT_IN},
        {NULL, TOKEN_OTHER}
    };

    int i = 0;
    while (redir_map[i].op_str != NULL)
    {
        size_t len = ft_strlen(redir_map[i].op_str);
        if (strncmp(line, redir_map[i].op_str, len) == 0)
            return redir_map[i].type;
        i++;
    }
    return TOKEN_OTHER;
}
t_token_type detect_type(const char *s, int quoted, t_token_type last_type)
{
    t_token_type type;

    if (!quoted)
    {
        type = get_redirection_type(s);
        if (type != TOKEN_OTHER)
            return type;

        type = get_operator_type(s);
        if (type != TOKEN_OTHER)
            return type;
    }

    // Déduction par contexte
    if (last_type == TOKEN_PIPE || last_type == TOKEN_OTHER)
        return TOKEN_COMMAND;

    if (last_type == TOKEN_REDIRECT_OUT || last_type == TOKEN_REDIRECT_APPEND ||
        last_type == TOKEN_REDIRECT_IN || last_type == TOKEN_HEREDOC)
        return TOKEN_ARGUMENT;

    return TOKEN_ARGUMENT;
}