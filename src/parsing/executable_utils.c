/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/07/31 22:57:34 by Hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_paths(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

int	check_file_in_list(t_list *files, char *str)
{
	while (files)
	{
		if (ft_strcmp(files->content, str) == 0)
			return (1);
		files = files->next;
	}
	return (0);
}

char	*search_in_paths(char **paths, char *str)
{
	t_list	*files;
	char	*result;
	int		i;

	i = 0;
	while (paths[i])
	{
		files = get_files_in_dir(paths[i]);
		if (!files)
		{
			free_paths(paths);
			return (NULL);
		}
		if (check_file_in_list(files, str))
		{
			result = ft_strdup(paths[i]);
			ft_lstclear(&files, free);
			free_paths(paths);
			return (result);
		}
		ft_lstclear(&files, free);
		i++;
	}
	return (NULL);
}
