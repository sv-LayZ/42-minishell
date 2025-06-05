#include "../../include/minishell.h"
/*Haby
Parsing
- recuperation des commandes
	- interpretations des operateurs
		- redirections, pipe, $...*/
        
        
/*expand special variable
echo $? return 0 success (if pattern found)
echo $? retun 1 if pattern not found
echo $? return 2 error for "no such file or directory", also if pattern not found
*/

/*expand_variables(line)
- Determine si la ligne contient des variables d'environnement 
- les remplace par leurs valeurs correspondantes.
*/

char *expand_variables(char *line)
{
	char *result = malloc(strlen(line) + 1);
	if (!result)
	{
		perror("malloc");
		return NULL;
	}

	int i = 0, j = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && (ft_isalnum(line[i + 1]) || line[i + 1] == '_'))
		{
			i++;
			char var_name[256];
			int k = 0;

			while (line[i] && (ft_isalnum(line[i]) || line[i] == '_') && k < 255)
			{
				var_name[k++] = line[i++];
			}
			var_name[k] = '\0';

			char *value = getenv(var_name);
			if (value)
			{
				strcpy(&result[j], value);
				j += strlen(value);
			}
		}
		else
		{
			result[j++] = line[i++];
		}
	}
	result[j] = '\0';

	return result;
}

t_token_type set_token_type()
{
		if (ft_strcmp(token->value, "|") == 0)
			token->type = TOKEN_PIPE; // Pipe
		else if (ft_strcmp(token->value, ">") == 0 || strcmp(token->value, ">>") == 0)
			token->type = TOKEN_REDIRECT_OUT; // Output redirection
		else if (ft_strcmp(token->value, "<") == 0 || strcmp(token->value, "<<") == 0)
			token->type = TOKEN_REDIRECT_IN; // Input redirection
		// else if (token->value[0] == '$')
			// token->type = TOKEN_ENV_VAR; // Variable
		else
			token->type = TOKEN_OTHER; // Normal token
}

/*fonction qui recupere les differentes commandes (token)
e*/
//strlen token->value
int str_tokenizer(char *line)
{
	t_token *token;
	token = malloc(sizeof(t_token));
	
    int count = 0;
    char *ifs = getenv("IFS");
    if (!ifs)
        ifs = " \t\n";

    char *expanded_line = expand_variables(line);
    if (!expanded_line)
        return -1;
    token->value = ft_split_str(expanded_line, ifs); // a modif
    free(expanded_line);
    if (!token->value)
    {
        perror("Error splitting line");
        return -1;
    }

    while (token->value[count])
    {
        set_token_type(token->value[count]);
        count++;
    }
    return 0;
}


// VAR="Hello world" ./parse "\"$VAR"\"
// Word 1: "Hello world"
