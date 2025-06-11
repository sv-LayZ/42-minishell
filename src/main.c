
#include "minishell.h"

int main(int ac, char **av)
{
	char	*line;
	
	(void)ac;
	(void)av;
	line = reader();
	while (line)
	{
		add_history(line);
		free(line);
		line = reader();
	}
	free(line);
}