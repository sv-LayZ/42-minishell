#include "../include/minishell.h"

int main(int ac, char **av)
{
	(void) ac;
	(void) av;
	char	*line;
	t_cmd	*cmds;

	handle_signals();  // Set up signal handlers ok
	while (1) 
	{
		line = reader();

		add_history(line); // Add the line to history ok
		cmds = parsing(line);
		print_commands(cmds); // DEBUG

		execute_builtin(is_builtin(cmds->args[0]), cmds->args); // DEBUG

		free_commands(cmds);
		free(line);
	}
}