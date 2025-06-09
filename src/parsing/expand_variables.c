#include "../../include/minishell.h"

char *expand_variables(const char *line)
{
    size_t result_size = 1024;
    char *result = malloc(result_size);
    if (!result)
        return NULL;

    size_t i = 0, j = 0;
    while (line[i])
    {
        if (line[i] == '$' && line[i + 1] && (ft_isalnum(line[i + 1]) || line[i + 1] == '_'))
        {
            i++;
            char var_name[256];
            int k = 0;

            while (line[i] && (ft_isalnum(line[i]) || line[i] == '_') && k < 255)
                var_name[k++] = line[i++];
            var_name[k] = '\0';

            char *value = getenv(var_name);
            if (value)
            {
                size_t val_len = strlen(value);
                while (j + val_len >= result_size)
                {
                    result_size *= 2;
                    char *new_result = realloc(result, result_size);
                    if (!new_result)
                    {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                strcpy(&result[j], value);
                j += val_len;
            }
        }
        else
        {
            // Expand buffer if needed
            if (j + 1 >= result_size)
            {
                result_size *= 2;
                char *new_result = realloc(result, result_size);
                if (!new_result)
                {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[j++] = line[i++];
        }
    }

    result[j] = '\0';
    return result;
}


// VAR="Hello world" ./parse "\"$VAR"\"
// Word 1: "Hello world"
