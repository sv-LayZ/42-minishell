/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mregnaut <mregnaut@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:29:30 by mregnaut          #+#    #+#             */
/*   Updated: 2025/05/27 15:49:36 by mregnaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>

char	*reader(void)
{
	char	*line;

	line = readline("Minishell> ");
	if (!line)
	{
		perror("Error reading line");
		return (NULL);
	}
	if (ft_strlen(line) == 0)
	{
		free(line);
		return (NULL);
	}
	add_history(line); // TODO: add own history
	return (line);
}