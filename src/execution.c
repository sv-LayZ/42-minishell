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

int	execute_command(t_cmd *cmd)
{
	int	builtin_index;
	int	exit_code;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	
	// Check if there are redirections
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