
#include "minishell.h"

int main(void)
{
	char	*line;
	t_cmd	*cmds;

	line = reader();
	while (line)
	{
		add_history(line);
		free(line);
		line = reader();

		cmds = parsing(line);
		print_commands(cmds); // DEBUG

		execute_builtin(is_builtin(cmds->args[0]), cmds->args); // DEBUG

		free_commands(cmds);
	}
	free(line);
}