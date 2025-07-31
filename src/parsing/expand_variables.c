/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/07/31 23:23:09 by Hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expand_variables(const char *line)
{
	size_t			result_size;
	char			*result;
	size_t			i;
	size_t			j;
	t_expand_data	data;

	result_size = 1024;
	result = malloc(result_size);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	data.result = &result;
	data.j = &j;
	data.result_size = &result_size;
	while (line[i])
	{
		if (!handle_dollar_sign(line, &i, &data))
			return (NULL);
	}
	result[j] = '\0';
	return (result);
}
