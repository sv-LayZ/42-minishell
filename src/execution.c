#include "../include/minishell.h"

extern char **environ;

static char	*find_executable_in_path(char *cmd)
{
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (!cmd || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	char *path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			break;
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (full_path && access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static int	setup_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (!cmd->input_file)
		return (0);
	if (cmd->heredoc)
	{
		// TODO: Implement heredoc later
		printf("minishell: heredoc not implemented yet\n");
		return (-1);
	}
	fd = open(cmd->input_file, O_RDONLY);
	if (fd == -1)
	{
		perror(cmd->input_file);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	setup_output_redirection(t_cmd *cmd)
{
	int	fd;
	int	flags;

	if (!cmd->output_file)
		return (0);
	flags = O_WRONLY | O_CREAT;
	if (cmd->append_output)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->output_file, flags, 0644);
	if (fd == -1)
	{
		perror(cmd->output_file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	execute_external_command(char **args)
{
	char	*executable_path;
	pid_t	pid;
	int		status;

	if (!args || !args[0])
		return (127);
	executable_path = find_executable_in_path(args[0]);
	if (!executable_path)
	{
		printf("minishell: %s: command not found\n", args[0]);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(executable_path);
		return (1);
	}
	if (pid == 0)
	{
		// Child process
		if (execve(executable_path, args, environ) == -1)
		{
			perror("execve");
			free(executable_path);
			exit(127);
		}
	}
	else
	{
		// Parent process
		free(executable_path);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (128 + WTERMSIG(status));
	}
	return (0);
}

static int	count_commands(t_cmd *cmd)
{
	int count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static int	execute_with_redirections(t_cmd *cmd)
{
	pid_t	pid;
	int		status;
	int		builtin_index;

	builtin_index = is_builtin(cmd->args[0]);
	if (builtin_index != -1)
	{
		// For builtins, handle redirections in child process to preserve parent's stdio
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}
		if (pid == 0)
		{
			// Child process
			if (setup_input_redirection(cmd) == -1)
				exit(1);
			if (setup_output_redirection(cmd) == -1)
				exit(1);
			exit(execute_builtin(builtin_index, cmd->args));
		}
		else
		{
			// Parent process
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				return (WEXITSTATUS(status));
			return (128 + WTERMSIG(status));
		}
	}
	else
	{
		// External command
		char *executable_path = find_executable_in_path(cmd->args[0]);
		if (!executable_path)
		{
			printf("minishell: %s: command not found\n", cmd->args[0]);
			return (127);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(executable_path);
			return (1);
		}
		if (pid == 0)
		{
			// Child process
			if (setup_input_redirection(cmd) == -1)
				exit(1);
			if (setup_output_redirection(cmd) == -1)
				exit(1);
			if (execve(executable_path, cmd->args, environ) == -1)
			{
				perror("execve");
				free(executable_path);
				exit(127);
			}
		}
		else
		{
			// Parent process
			free(executable_path);
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				return (WEXITSTATUS(status));
			return (128 + WTERMSIG(status));
		}
	}
	return (0);
}

static int	execute_single_cmd_in_pipeline(t_cmd *cmd, int input_fd, int output_fd, int **pipes, int cmd_count)
{
	pid_t	pid;
	int		builtin_index;
	int		i;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		// Child process
		// Close all pipe file descriptors except the ones we need
		for (i = 0; i < cmd_count - 1; i++)
		{
			if (pipes[i][0] != input_fd)
				close(pipes[i][0]);
			if (pipes[i][1] != output_fd)
				close(pipes[i][1]);
		}
		
		// Set up input redirection
		if (input_fd != STDIN_FILENO)
		{
			if (dup2(input_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 input");
				exit(1);
			}
			close(input_fd);
		}
		
		// Set up output redirection
		if (output_fd != STDOUT_FILENO)
		{
			if (dup2(output_fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 output");
				exit(1);
			}
			close(output_fd);
		}
		
		// Handle file redirections
		if (setup_input_redirection(cmd) == -1)
			exit(1);
		if (setup_output_redirection(cmd) == -1)
			exit(1);
		
		// Execute the command
		builtin_index = is_builtin(cmd->args[0]);
		if (builtin_index != -1)
		{
			exit(execute_builtin(builtin_index, cmd->args));
		}
		else
		{
			char *executable_path = find_executable_in_path(cmd->args[0]);
			if (!executable_path)
			{
				printf("minishell: %s: command not found\n", cmd->args[0]);
				exit(127);
			}
			if (execve(executable_path, cmd->args, environ) == -1)
			{
				perror("execve");
				free(executable_path);
				exit(127);
			}
		}
	}
	return (pid);
}

static int	execute_pipeline(t_cmd *cmd_list)
{
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	int		i;
	int		status;
	int		last_exit_status = 0;
	t_cmd	*current;

	cmd_count = count_commands(cmd_list);
	if (cmd_count == 1)
	{
		// Single command, no pipes needed
		return (execute_with_redirections(cmd_list));
	}

	// Allocate memory for pipes and process IDs
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pipes || !pids)
	{
		perror("malloc");
		return (1);
	}

	// Create all pipes
	for (i = 0; i < cmd_count - 1; i++)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return (1);
		}
	}

	// Execute each command in the pipeline
	current = cmd_list;
	for (i = 0; i < cmd_count; i++)
	{
		int input_fd = STDIN_FILENO;
		int output_fd = STDOUT_FILENO;

		// Set input from previous pipe
		if (i > 0)
			input_fd = pipes[i - 1][0];

		// Set output to next pipe
		if (i < cmd_count - 1)
			output_fd = pipes[i][1];

		pids[i] = execute_single_cmd_in_pipeline(current, input_fd, output_fd, pipes, cmd_count);
		current = current->next;
	}

	// Close all pipe file descriptors in parent
	for (i = 0; i < cmd_count - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	// Wait for all child processes
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1) // Last command determines exit status
		{
			if (WIFEXITED(status))
				last_exit_status = WEXITSTATUS(status);
			else
				last_exit_status = 128 + WTERMSIG(status);
		}
	}

	// Free allocated memory
	for (i = 0; i < cmd_count - 1; i++)
		free(pipes[i]);
	free(pipes);
	free(pids);
	return (last_exit_status);
}

int	execute_command(t_cmd *cmd)
{
	int	builtin_index;
	int	exit_code;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	
	// Check if this is a pipeline (multiple commands)
	if (cmd->next)
	{
		return (execute_pipeline(cmd));
	}
	
	// Single command - check if there are redirections
	if (cmd->input_file || cmd->output_file)
	{
		return (execute_with_redirections(cmd));
	}
	
	// No redirections, execute normally
	builtin_index = is_builtin(cmd->args[0]);
	if (builtin_index != -1)
	{
		exit_code = execute_builtin(builtin_index, cmd->args);
	}
	else
	{
		exit_code = execute_external_command(cmd->args);
	}
	return (exit_code);
} 