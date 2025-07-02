#include "../include/minishell.h"

int main(void)
{
	char	*line;
	t_cmd	*cmds;

	(void)ac;
	(void)av;
	handle_signals();  // Set up signal handlers ok
	while (1) 
	{
		line = reader();

		cmds = parsing(line);
		print_commands(cmds); // DEBUG

		execute_builtin(is_builtin(cmds->args[0]), cmds->args); // DEBUG

		free_commands(cmds);
		free(line);
	}
}