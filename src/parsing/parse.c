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

int get_token_type(t_token *token)
{
    if (ft_strcmp(token, "|") == 0)
        token->type = TOKEN_PIPE; // Pipe
    else if (ft_strcmp(token, ">") == 0 || strcmp(token, ">>") == 0)
        token->type = TOKEN_REDIRECT_OUT; // Output redirection
    else if (ft_strcmp(token, "<") == 0 || strcmp(token, "<<") == 0)
        token->type = TOKEN_REDIRECT_IN; // Input redirection
    else if (token->value[0] == '$')
        token->type = TOKEN_ENV_VAR; // Variable
    else
        return 0; // Normal token
}

/*fonction qui recupere les differentes commandes (token)
e*/
//strlen token->value
int str_tokenizer(char *line, char **words, int *word_count)
{
    char **tokens;
    int count = 0;
    char *ifs = getenv("IFS");
    if (!ifs)
        ifs = " \t\n";

    char *expanded_line = expand_variables(line);
    if (!expanded_line)
        return -1;
    tokens = ft_split_str(expanded_line, ifs); // a modif
    free(expanded_line);
    if (!tokens)
    {
        perror("Error splitting line");
        return -1;
    }

    while (tokens[count])
    {
        get_token_type(tokens[count]);
        count++;
    }
    return 0;
}

// Fix for main() function
int main(int argc, char **argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		return 1;
	}

	char *line = argv[1];
	char *expanded_line = expand_variables(line);
	if (!expanded_line)
		return 1;

	printf("Expanded line: %s\n", expanded_line);
	free(expanded_line);

	char *words[100];
	int word_count = 0;
	if (str_tokenizer(line, words, &word_count) == -1)
		return 1;

	for (int i = 0; i < word_count; i++)
		printf("Word %d: %s\n", i + 1, words[i]);

	return 0;
}
// VAR="Hello world" ./parse "\"$VAR"\"
// Word 1: "Hello world"
