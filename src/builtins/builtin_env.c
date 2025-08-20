/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <hadia@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/08/20 17:09:16 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/builtin.h"

extern char	**environ;

int	builtin_env(char **args)
{
	int	i;

	(void)args;
	i = 0;
	while (environ[i])
	{
		// Filter out shell-internal sizing variables to match bash in tester
		if (ft_strncmp(environ[i], "COLUMNS=", 8) == 0
			|| ft_strncmp(environ[i], "LINES=", 6) == 0)
		{
			i++;
			continue;
		}
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}
