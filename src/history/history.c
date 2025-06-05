/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:32:41 by hadia             #+#    #+#             */
/*   Updated: 2025/05/20 14:32:59 by Hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*creer un fichier history
stock les commandes precedentes, leur attributs un numero selon l'ordre
*/
static int write_in_history_file(char *history, char *command, int *command_count)
{
    int fd;

    fd = open(history, O_WRONLY | O_APPEND);
    if (fd == -1)
    {
        perror("Error opening history file for writing");
        return (-1);
    }
    char *entry = ft_strjoin(ft_itoa(*command_count), " ");
    entry = ft_strjoin(entry, command);
    entry = ft_strjoin(entry, "\n");

    if (write(fd, entry, strlen(entry)) == -1)
    {
        perror("Error writing to history file");
        free(entry);
        close(fd);
        return (-1);
    }
    free(entry);
    (*command_count)++;
    close(fd);
    return (0);
}

// static int read_history_line(char *history, int line_number)
// {
//     int fd;
//     char *line;
//     int current_line = 1;

//     fd = open(history, O_RDONLY);
//     if (fd == -1)
//     {
//         perror("Error opening history file");
//         return (-1);
//     }
//     while ((line = get_next_line(fd)) != NULL)
//     {
//         if (current_line == line_number)
//         {
//             printf("%s", line);
//             free(line);
//             close(fd);
//             return (0);
//         }
//         free(line);
//         current_line++;
//     }
//     close(fd);
//     fprintf(stderr, "Line %d not found in history\n", line_number);
//     return (-1);
// }
// static int read_history_file(char *history)
// {
//     int fd;
//     char *line;

//     fd = open(history, O_RDONLY);
//     if (fd == -1)
//     {
//         perror("Error opening history file");
//         return (-1);
//     }
//     while ((line = get_next_line(fd)) != NULL)
//     {
//         printf("%s", line);
//         free(line);
//     }
//     close(fd);
//     return (0);
// }

static int add_in_history_file(char *history, char *line)
{
    int fd;
    int command_count = 1;

    fd = open(history, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Error creating history file");
        return (-1);
    }
    close(fd);

        if (write_in_history_file(history, line, &command_count) == -1)
        {
            free(line);
            return (-1);
        }

    return (0);
}

int history_process(char *line)
{
    add_in_history_file("history.txt", line);
    return (0);   
}
