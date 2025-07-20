#include "../include/minishell.h"

// Global variable for exit status ($?)
int g_exit_status = 0;

int main(int ac, char **av)
{
	(void) ac;
	(void) av;
	char	*line;
	t_cmd	*cmds;

	handle_signals();  // Set up signal handlers
	while (1) 
	{
		line = reader();
		if (!line)  // Handle empty input (Ctrl+D handled in reader())
			continue;

		if (ft_strlen(line) > 0)
			add_history(line); // Add non-empty lines to history
		
		cmds = parsing(line);
		if (cmds && cmds->args && cmds->args[0])
		{
			g_exit_status = execute_command(cmds);
		}

		free_commands(cmds);
		free(line);
	}
	return (0);
}