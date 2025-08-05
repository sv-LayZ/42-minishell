/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_token_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvbyteam <hvbyteam@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 00:00:00 by hvbyteam          #+#    #+#             */
/*   Updated: 2025/08/06 00:00:00 by hvbyteam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	handle_quoted_part(const char *input, int *i)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
}

char	*extract_quoted_token(const char *input, int *i)
{
	int		start;
	char	quote;
	int		len;
	char	*token;

	start = *i;
	quote = input[(*i)++];
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
	len = *i - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, &input[start], len);
	token[len] = '\0';
	return (token);
}

char	*extract_operator_token(const char *input, int *i)
{
	int		len;
	char	*token;

	len = operator_token_length(&input[*i]);
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, &input[*i], len);
	token[len] = '\0';
	*i += len;
	return (token);
}

char	*extract_word_token(const char *input, int *i)
{
	int		start;
	int		len;
	char	*token;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
			handle_quoted_part(input, i);
		else
			(*i)++;
	}
	len = *i - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, &input[start], len);
	token[len] = '\0';
	return (token);
}
