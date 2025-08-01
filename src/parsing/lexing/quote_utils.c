/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvbyteam <hvbyteam@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:00:00 by hvbyteam          #+#    #+#             */
/*   Updated: 2025/07/31 00:00:00 by hvbyteam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_quote_type	get_quote_type(const char *value)
{
	size_t	len;

	if (!value)
		return (NO_QUOTE);
	len = strlen(value);
	if (len >= 2 && value[0] == '\'' && value[len - 1] == '\'')
		return (SINGLE_QUOTE);
	else if (len >= 2 && value[0] == '"' && value[len - 1] == '"')
		return (DOUBLE_QUOTE);
	return (NO_QUOTE);
}

char	*remove_quotes(const char *str)
{
	size_t	len;
	char	*res;

	if (!str)
		return (NULL);
	len = strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')))
	{
		res = malloc(len - 1);
		if (!res)
			return (NULL);
		strncpy(res, str + 1, len - 2);
		res[len - 2] = '\0';
		return (res);
	}
	return (strdup(str));
}
