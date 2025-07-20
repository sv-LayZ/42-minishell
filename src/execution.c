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
	paths = ft_split(getenv("PATH"), ':');
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
		{
			// Free paths array
			while (paths[i])
				free(paths[i++]);
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	// Free paths array
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (NULL);
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

int	execute_command(t_cmd *cmd)
{
	int	builtin_index;
	int	exit_code;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	
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