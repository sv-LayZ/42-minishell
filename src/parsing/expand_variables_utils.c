/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/07/31 23:23:09 by Hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expand_exit_status(char **result, size_t *j, size_t *result_size)
{
	char	*exit_status_str;
	char	*new_result;
	size_t	val_len;

	exit_status_str = ft_itoa(g_exit_status);
	if (!exit_status_str)
		return (NULL);
	val_len = strlen(exit_status_str);
	while (*j + val_len >= *result_size)
	{
		*result_size *= 2;
		new_result = realloc(*result, *result_size);
		if (!new_result)
		{
			free(*result);
			free(exit_status_str);
			return (NULL);
		}
		*result = new_result;
	}
	strcpy(&(*result)[*j], exit_status_str);
	*j += val_len;
	free(exit_status_str);
	return (*result);
}

static char	*extract_var_name(const char *line, size_t *i)
{
	size_t	start;
	size_t	len;
	char	*var_name;

	start = *i;
	len = 0;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
	{
		(*i)++;
		len++;
	}
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, &line[start], len + 1);
	return (var_name);
}

static int	expand_env_var(char *var_name, t_expand_data *data)
{
	char	*env_value;
	char	*new_result;
	size_t	val_len;

	env_value = getenv(var_name);
	if (!env_value)
		env_value = "";
	val_len = strlen(env_value);
	while (*(data->j) + val_len >= *(data->result_size))
	{
		*(data->result_size) *= 2;
		new_result = realloc(*(data->result), *(data->result_size));
		if (!new_result)
		{
			free(*(data->result));
			return (0);
		}
		*(data->result) = new_result;
	}
	strcpy(&(*(data->result))[*(data->j)], env_value);
	*(data->j) += val_len;
	return (1);
}

static int	handle_variable_expansion(const char *line, size_t *i,
			t_expand_data *data)
{
	char	*var_name;

	if (line[*i] == '?')
	{
		(*i)++;
		if (!expand_exit_status(data->result, data->j, data->result_size))
			return (0);
	}
	else if (ft_isalpha(line[*i]) || line[*i] == '_')
	{
		var_name = extract_var_name(line, i);
		if (!var_name)
			return (0);
		if (!expand_env_var(var_name, data))
		{
			free(var_name);
			return (0);
		}
		free(var_name);
	}
	else
		(*(data->result))[(*(data->j))++] = '$';
	return (1);
}

int	handle_dollar_sign(const char *line, size_t *i, t_expand_data *data)
{
	if (line[*i] == '$' && line[*i + 1])
	{
		(*i)++;
		if (!handle_variable_expansion(line, i, data))
			return (0);
	}
	else
		(*(data->result))[(*(data->j))++] = line[(*i)++];
	return (1);
}
