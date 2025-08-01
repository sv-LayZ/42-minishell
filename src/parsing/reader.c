/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:29:30 by mregnaut          #+#    #+#             */
/*   Updated: 2025/07/31 22:59:07 by Hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*reader(void)
{
	char	*line;

	line = readline("Minishell> ");
	if (line == NULL)
		exit(0);
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
	return (line);
}
