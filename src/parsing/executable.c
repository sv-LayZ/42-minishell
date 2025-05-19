#include "../../include/minishell.h"

static void	*free_list(t_list *files, void *content, char *error)
{
	if (error)
		perror(error);
	if (files)
		ft_lstclear(&files, free);
	if (content)
		free(content);
	return (NULL);
}

static t_list	*list_files(DIR *dirp)
{
	struct dirent	*dir;
	char			*file;
	int 			errsv;
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

static t_list	*get_files_in_dir(char *path)
{
	DIR				*dirp;
	t_list			*files;
	int				errsv;

	errsv = 0;
	files = NULL;
	dirp = opendir(path);
	if (!dirp)
	{
		errsv = errno;
		perror(strerror(errsv));
		return (NULL);
	}
	files = list_files(dirp);
	if (!files)
		perror(ERROR_READING_DIR);
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
	t_list	*files;
	int		i;
	
	paths = ft_split(getenv(ENV_PATH), ENV_SEPARATOR);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		files = get_files_in_dir(paths[i]);
		if (!files)
			return (ft_free_tab(paths), NULL);
		while (files)
		{
			if (ft_strncmp(str, files->content, ft_strlen(files->content)) == 0)
			{
				ft_lstclear(&files, free);
				return (paths[i]);
			}
			files = files->next;
		}
		ft_lstclear(&files, free);
		i++;
	}
}

// int main(int argc, char **argv)
// {
// 	// t_list *files = get_files_in_dir(argv[1]);
// 	// if (files)
// 	// {
// 	// 	t_list *temp = files;
// 	// 	while (temp)
// 	// 	{
// 	// 		__builtin_printf("%s\n", (char *)temp->content);
// 	// 		temp = temp->next;
// 	// 	}
// 	// 	ft_lstclear(&files, free);
// 	// }
// 	// else
// 	// {
// 	// 	perror("Error");
// 	// }

// 	char *path = get_executable_path(argv[1]);
// 	if (path)
// 	{
// 		__builtin_printf("Executable path: %s/%s\n", path, argv[1]);
// 		free(path);
// 	}
// 	else
// 	{
// 		perror("Error");
// 	}
// }