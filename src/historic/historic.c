/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   historic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <hadia@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:32:41 by hadia             #+#    #+#             */
/*   Updated: 2025/05/13 19:11:43 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*creer un fichier history
stock les commandes precedentes, leur attributs un numero selon l'ordre
*/
int create_history_file(char *history, char *path)
{
	int fd;

	fd = open(history, O_CREAT | O_RDWR);
	if (fd == -1)
	{
		perror("Error creating history file");
		return (-1);
	}
	while(1)
	{
		char *line = readline("Minishell> ");
		if (!line)
			break;
		if (write(fd, line, strlen(line)) == -1)
		{
			perror("Error writing to history file");
			free(line);
			close(fd);
			return (-1);
		}
		free(line);
		if (write(fd, "\n", 1) == -1)
		{
			perror("Error writing newline to history file");
			close(fd);
			return (-1);
		}
	}
	close(fd);
	return (0);
}
int read_history_line(int fd, char *nb_line)
{
	char *line;
	int nb_line_size;

	nb_line_size = ft_strlen(ft_itoa(nb_line));
	fd = open("history.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening history file");
		return (-1);
	}
	/*trouver nu*/
	ft_strnstr(line, nb_line, nb_line_size)
	while (*line)
	{
		line = get_next_line(fd);
		if (line == NULL)
		{
			close(fd);
			return (-1);
		}
		free(line);
		nb_line--;
	}
}
int read_history_file(int fd, char *command)
{
	char *line;

	/*if command = "history"*/
	if(command == "history")
	{
		fd = open("history.txt", O_RDONLY);
		if (fd == -1)
		{
			perror("Error opening history file");
			return (-1);
		}
		while ((line = get_next_line(fd)) != NULL)
		{
			printf("%s", line);
			free(line);
		}
	}
	close(fd);
	return (0);
}


