/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/08/22 14:33:24 by Hadia            ###   ########.fr       */
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

int	setup_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (!cmd->input_file)
		return (0);
	if (cmd->heredoc)
	{
		ft_putstr_fd("minishell: heredoc not implemented yet\n", 2);
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

/* Apply ordered redirections list (left-to-right). Return 0 success, 1 error. */
int	apply_redirections(t_cmd *cmd)
{
	t_redir *r;
	int	fd;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == R_HEREDOC)
		{
			ft_putstr_fd("minishell: heredoc not implemented yet\n", 2);
			return (1);
		}
		if (r->type == R_IN)
		{
			fd = open(r->filename, O_RDONLY);
			if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
			{
				perror(r->filename);
				if (fd != -1)
					close(fd);
				return (1);
			}
			close(fd);
		}
		else if (r->type == R_OUT_TRUNC || r->type == R_OUT_APPEND)
		{
			fd = open(r->filename, O_WRONLY | O_CREAT | (r->type == R_OUT_APPEND ? O_APPEND : O_TRUNC), 0644);
			if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
			{
				perror(r->filename);
				if (fd != -1)
					close(fd);
				return (1);
			}
			close(fd);
		}
		r = r->next;
	}
	return (0);
}
