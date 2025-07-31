/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/07/31 22:57:49 by Hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	*free_list(t_list *files, void *content, char *error)
{
	if (error)
		perror(error);
	(void)files;
	(void)content;
	return (NULL);
}

static t_list	*list_files(DIR *dirp)
{
	struct dirent	*dir;
	char			*file;
	int				errsv;
	t_list			*files;
	t_list			*temp;

	errsv = 0;
	files = NULL;
	dir = readdir(dirp);
	while (dir != NULL)
	{
		file = ft_strdup(dir->d_name);
		if (!file)
			return (free_list(files, file, ERROR_MEMORY));
		temp = ft_lstnew(file);
		if (!temp)
			return (free_list(files, file, ERROR_MEMORY));
		ft_lstadd_back(&files, temp);
		dir = readdir(dirp);
	}
	errsv = errno;
	if (errsv != 0)
		return (free_list(files, file, strerror(errsv)));
	return (files);
}

t_list	*get_files_in_dir(char *path)
{
	DIR				*dirp;
	t_list			*files;
	int				errsv;

	errsv = 0;
	files = NULL;
	dirp = opendir(path);
	if (!dirp)
	{
		perror("opendir (get_files_in_dir)");
		return (NULL);
	}
	files = list_files(dirp);
	if (!files)
		perror("list_files (get_files_in_dir)");
	if (closedir(dirp) == -1)
	{
		errsv = errno;
		return (free_list(files, NULL, strerror(errsv)));
	}
	return (files);
}

char	*get_executable_path(char *str)
{
	char	**paths;
	char	*result;

	paths = ft_split(getenv(ENV_PATH), ENV_SEPARATOR);
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, str);
	if (!result)
		free_paths(paths);
	return (result);
}
