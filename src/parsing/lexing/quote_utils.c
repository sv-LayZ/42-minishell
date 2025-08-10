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

static int	check_quote_presence(const char *value, size_t len)
{
	size_t	i;
	int		has_single;
	int		has_double;

	has_single = 0;
	has_double = 0;
	i = 0;
	while (i < len)
	{
		if (value[i] == '\'')
			has_single = 1;
		else if (value[i] == '"')
			has_double = 1;
		i++;
	}
	if (len >= 2 && value[0] == '\'' && value[len - 1] == '\''
		&& !has_double)
		return (1);
	else if (len >= 2 && value[0] == '"' && value[len - 1] == '"'
		&& !has_single)
		return (2);
	else if (has_single || has_double)
		return (3);
	return (0);
}

t_quote_type	get_quote_type(const char *value)
{
	size_t	len;
	int		quote_check;

	if (!value)
		return (NO_QUOTE);
	len = strlen(value);
	quote_check = check_quote_presence(value, len);
	if (quote_check == 1)
		return (SINGLE_QUOTE);
	else if (quote_check == 2)
		return (DOUBLE_QUOTE);
	else if (quote_check == 3)
		return (DOUBLE_QUOTE);
	return (NO_QUOTE);
}

static void	process_quoted_section(const char *str, t_quote_data *data)
{
	char	quote;

	quote = str[*(data->i)];
	(*(data->i))++;
	while (*(data->i) < data->len && str[*(data->i)] != quote)
	{
		data->res[*(data->j)] = str[*(data->i)];
		(*(data->j))++;
		(*(data->i))++;
	}
	if (*(data->i) < data->len && str[*(data->i)] == quote)
		(*(data->i))++;
}

static char	*init_result_buffer(const char *str, size_t *len)
{
	char	*res;

	if (!str)
		return (NULL);
	*len = strlen(str);
	res = malloc(*len + 1);
	return (res);
}

char	*remove_quotes(const char *str)
{
	size_t			len;
	char			*res;
	size_t			i;
	size_t			j;
	t_quote_data	data;

	res = init_result_buffer(str, &len);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	data = (t_quote_data){&i, res, &j, len};
	while (i < len)
	{
		if (str[i] == '"' || str[i] == '\'')
			process_quoted_section(str, &data);
		else
		{
			res[j] = str[i];
			j++;
			i++;
		}
	}
	res[j] = '\0';
	return (res);
}
