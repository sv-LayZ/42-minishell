/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <hadia@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:29:30 by mregnaut          #+#    #+#             */
/*   Updated: 2025/06/23 21:28:58 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*reader(void)
{
	char	*line;
	//t_cmd	*cmds;

	line = readline("Minishell> ");
	if (line == NULL) //for ctrl-d
		exit(0);
	if (!line)
	{
		perror("Error reading line");
		 return (NULL);
		// exit(0);
	}
	if (ft_strlen(line) == 0)
	{
		free(line);
		return (NULL);
	}
	//cmds = parsing(line); // Parse the input line into commands
	//print_commands(cmds);
 //free_commands(cmds);
	return (line);
}