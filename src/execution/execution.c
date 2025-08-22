/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 21:50:01 by Hadia             #+#    #+#             */
/*   Updated: 2025/08/22 14:33:24 by Hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/stat.h>

extern char **environ;

static int classify_and_exec_single(char **args)
{
	struct stat st;
	int		builtin_index;
	char	*path;
	int		need_free = 0;

	if (!args || !args[0])
		return (0);
	builtin_index = is_builtin(args[0]);
	if (builtin_index != -1)
		return (execute_builtin(builtin_index, args));
	/* path resolution */
	if (ft_strchr(args[0], '/'))
	{
		path = args[0];
	}
	else
	{
		path = find_executable_in_path(args[0]);
		need_free = (path != NULL && path != args[0]);
	}
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	if (stat(path, &st) == -1)
	{
		perror(path);
		if (need_free)
			free(path);
		return (127);
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		if (need_free)
			free(path);
		return (126);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		if (need_free)
			free(path);
		return (126);
	}
	/* exec external */
	return (execute_external_command(args));
}

/* child path resolution + exec for pipeline (builtin or external) */
static void	child_exec_pipeline(t_cmd *cmd)
{
	int		b_index;
	char	*path;
	int		need_free;
	struct stat st;

	if (!cmd->args || !cmd->args[0])
		exit(0);
	b_index = is_builtin(cmd->args[0]);
	if (b_index != -1)
		exit(execute_builtin(b_index, cmd->args));
	/* external */
	need_free = 0;
	if (ft_strchr(cmd->args[0], '/'))
		path = cmd->args[0];
	else
	{
		path = find_executable_in_path(cmd->args[0]);
		need_free = (path != NULL && path != cmd->args[0]);
	}
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (stat(path, &st) == -1)
	{
		perror(path);
		if (need_free)
			free(path);
		exit(127);
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		if (need_free)
			free(path);
		exit(126);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		if (need_free)
			free(path);
		exit(126);
	}
	execve(path, cmd->args, environ);
	/* only reached if execve fails */
	perror(path);
	if (need_free)
		free(path);
	exit(127);
}

static int execute_pipeline(t_cmd *cmd)
{
	int		count;
	t_cmd	*tmp;
	int		(*pipes)[2];
	int		i;
	pid_t	pid;
	int		status;
	int		last_status = 0;

	count = 0;
	tmp = cmd;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	pipes = NULL;
	if (count > 1)
	{
		pipes = malloc(sizeof(int[2]) * (count - 1));
		if (!pipes)
			return (1);
		i = 0;
		while (i < count - 1)
		{
			if (pipe(pipes[i]) == -1)
			{
				perror("pipe");
				while (i-- > 0)
				{
					close(pipes[i][0]);
					close(pipes[i][1]);
				}
				free(pipes);
				return (1);
			}
			i++;
		}
	}
	i = 0;
	tmp = cmd;
	while (tmp)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}
		if (pid == 0)
		{
			/* set up pipe ends */
			if (count > 1)
			{
				if (i > 0 && dup2(pipes[i-1][0], STDIN_FILENO) == -1)
					perror("dup2");
				if (i < count - 1 && dup2(pipes[i][1], STDOUT_FILENO) == -1)
					perror("dup2");
				int k = 0;
				while (k < count - 1)
				{
					close(pipes[k][0]);
					close(pipes[k][1]);
					k++;
				}
			}
			if (apply_redirections(tmp) != 0)
				exit(1);
			child_exec_pipeline(tmp);
		}
		tmp = tmp->next;
		i++;
	}
	if (count > 1)
	{
		i = 0;
		while (i < count - 1)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			i++;
		}
		free(pipes);
	}
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
	}
	return (last_status);
}

int	execute_command(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (cmd->next)
		return (execute_pipeline(cmd));
	/* single command: apply redirs then run */
	if (cmd->redirs)
	{
		pid_t pid = fork();
		int status;
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}
		if (pid == 0)
		{
			if (apply_redirections(cmd) != 0)
				exit(1);
			exit(classify_and_exec_single(cmd->args));
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (128 + WTERMSIG(status));
	}
	return (classify_and_exec_single(cmd->args));
}
