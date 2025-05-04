#include "../../include/minishell.h"
/*Haby
Parsing
- recuperation des commandes
	- interpretations des operateurs
		- redirections, pipe, $...*/


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


/*fonction qui recupere les differentes commandes (token)
e*/
int word_splitting(char *line, char **words, int *word_count)
{
    char **tokens;
    int count = 0;
    char *ifs = getenv("IFS");
    if (!ifs)
        ifs = " \t\n";

    char *expanded_line = expand_variables(line);
    if (!expanded_line)
        return -1;

    tokens = ft_split_ifs(expanded_line, ifs); 
    free(expanded_line);
    
    if (!tokens)
    {
        perror("Error splitting line");
        return -1;
    }
    
    // Create deep copies of the strings
    while (tokens[count]) {
        words[count] = ft_strdup(tokens[count]);  // Make copies of the strings
        if (!words[count]) {
            // Handle allocation failure
			int i = 0;
            while (i++ < count)
                free(words[i]);
            ft_free_tab(tokens);
            return -1;
        }
        count++;
    }
    
    words[count] = NULL;
    *word_count = count;

    ft_free_tab(tokens);  // Now safe to free
    return 0;
}


int main( int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: %s <string>\n", argv[0]);
		return 1;
	}

	char *line = argv[1];
	char *words[100]; // Adjust size as needed
	int word_count = 0;

	if (word_splitting(line, words, &word_count) == -1)
	{
		return 1;
	}

	for (int i = 0; i < word_count; i++)
	{
		printf("Word %d: %s\n", i + 1, words[i]);
	}

	return 0;
}
// VAR="Hello world" ./parse "\"$VAR"\"
// Word 1: "Hello world"
