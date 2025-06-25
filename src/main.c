#include "../include/minishell.h"

int main(int ac, char **av)
{
	char	*line;
	
	(void)ac;
	(void)av;
	handle_signals();  // Set up signal handlers ok
	while (1) 
	{
		line = reader(); 
		// TODO: action here
		// __builtin_printf("line: $%s$\n", line);
		// __builtin_printf("line: $%s$\n", get_executable_path("echo"));
		//__builtin_printf("executable: %s\n", get_executable_path(line));
		free(line);
  }
}