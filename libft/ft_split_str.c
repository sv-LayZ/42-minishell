#include "libft.h"

/*fonction verifie si c est dans l'ensemble des delimiteurs*/
static int is_delimiter(char c, char *delimiters)
{
    while (*delimiters)
    {
        if (c == *delimiters)
            return (1);
        delimiters++;
    }
    return (0);
}

/*fonction compte nombre mot dans s avec plusieurs delimiteurs en respectant les guillemets*/
static int ft_count_word_ifs(char *s, char *delimiters)
{
    int count = 0;
    int in_word = 0;
    char quote_char = 0;  // 0 = not in quotes, ' or " = in that type of quote
    
    while (*s)
    {
        // Handle quotes
        if ((*s == '\'' || *s == '"') && !quote_char)
        {
            quote_char = *s;  // Start quote
            if (!in_word)
            {
                in_word = 1;
                count++;
            }
        }
        else if (*s == quote_char)
        {
            quote_char = 0;  // End quote
        }
        else if (!quote_char && !is_delimiter(*s, delimiters) && !in_word)
        {
            // Not in quotes and found a new word
            in_word = 1;
            count++;
        }
        else if (!quote_char && is_delimiter(*s, delimiters))
        {
            // Not in quotes and found a delimiter
            in_word = 0;
        }
        s++;
    }
    return count;
}

/*fonction complete tab a l'aide de substr avec plusieurs delimiteurs et respect des guillemets*/
static int write_split_str(char **tab, char const *s, char *delimiters)
{
    int start;
    int i = 0;
    int j = 0;
    char quote_char = 0;
    
    while (s[i])
    {
        // Skip delimiters (unless in quotes)
        while (s[i] && is_delimiter(s[i], delimiters) && !quote_char)
            i++;
            
        start = i;
        
        // Process word with quotes
        while (s[i])
        {
            if ((s[i] == '\'' || s[i] == '"') && !quote_char)
                quote_char = s[i];
            else if (s[i] == quote_char)
                quote_char = 0;
            else if (is_delimiter(s[i], delimiters) && !quote_char)
                break;
            i++;
        }
        
        if (i > start)
        {
            tab[j] = ft_substr(s, start, i - start);
            if (!tab[j])
            {
                ft_free_tab(tab);
                return (0);
            }
            j++;
        }
    }
    return (1);
}

void ft_free_tab(char **tab)
{
    int i;

    i = 0;
    while (tab[i])
        free(tab[i++]);
    free(tab);
}
/*fonction split avec plusieurs delimiteurs (IFS)*/
char **ft_split_str(char const *s, char *delimiters)
{
    size_t words;
    char **tab;

    if (!s)
        return (NULL);
    words = ft_count_word_ifs((char *)s, delimiters);
    tab = malloc(sizeof(char *) * (words + 1));
    if (!tab)
        return (NULL);
    if (!write_split_ifs(tab, s, delimiters))
    {
        return (NULL);
    }
    tab[words] = NULL;
    return (tab);
}