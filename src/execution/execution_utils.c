/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mattm <mattm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/08/07 17:53:18 by mattm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			break ;
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (full_path && access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_executable_in_path(char *cmd)
{
	char	**paths;
	char	*path_env;
	char	*result;

	if (!cmd || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, cmd);
	return (result);
}

static char	*create_temp_filename(void)
{
	static int	counter = 0;
	char		*filename;
	char		*counter_str;
	char		*temp;

	counter_str = ft_itoa(counter++);
	if (!counter_str)
		return (NULL);
	temp = ft_strjoin("/tmp/minishell_heredoc_", counter_str);
	free(counter_str);
	if (!temp)
		return (NULL);
	counter_str = ft_itoa(getpid());
	if (!counter_str)
	{
		free(temp);
		return (NULL);
	}
	filename = ft_strjoin(temp, counter_str);
	free(temp);
	free(counter_str);
	return (filename);
}

static int	read_heredoc_content(const char *delimiter, int fd)
{
	char	*line;
	size_t	delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	ft_putstr_fd("> ", 1);
	line = readline("");
	while (line != NULL)
	{
		if (ft_strncmp(line, delimiter, delimiter_len) == 0
			&& line[delimiter_len] == '\0')
		{
			free(line);
			break ;
		}
		if (write(fd, line, ft_strlen(line)) == -1)
		{
			free(line);
			return (-1);
		}
		if (write(fd, "\n", 1) == -1)
		{
			free(line);
			return (-1);
		}
		free(line);
		ft_putstr_fd("> ", 1);
		line = readline("");
	}
	return (0);
}

static int	handle_heredoc(t_cmd *cmd)
{
	char	*temp_filename;
	int		temp_fd;
	int		read_fd;

	temp_filename = create_temp_filename();
	if (!temp_filename)
		return (-1);
	temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_fd == -1)
	{
		perror("open");
		free(temp_filename);
		return (-1);
	}
	if (read_heredoc_content(cmd->input_file, temp_fd) == -1)
	{
		close(temp_fd);
		unlink(temp_filename);
		free(temp_filename);
		return (-1);
	}
	close(temp_fd);
	read_fd = open(temp_filename, O_RDONLY);
	if (read_fd == -1)
	{
		perror("open");
		unlink(temp_filename);
		free(temp_filename);
		return (-1);
	}
	if (dup2(read_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(read_fd);
		unlink(temp_filename);
		free(temp_filename);
		return (-1);
	}
	close(read_fd);
	unlink(temp_filename);
	free(temp_filename);
	return (0);
}

int	setup_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (!cmd->input_file)
		return (0);
	if (cmd->heredoc)
	{
		return (handle_heredoc(cmd));
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

int	setup_output_redirection(t_cmd *cmd)
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
