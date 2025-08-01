/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 21:35:47 by mregnaut          #+#    #+#             */
/*   Updated: 2025/07/31 21:44:58 by Hadia            ###   ########.fr       */
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
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}
