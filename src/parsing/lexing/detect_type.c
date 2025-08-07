/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvbyteam <hvbyteam@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:00:00 by hvbyteam          #+#    #+#             */
/*   Updated: 2025/07/31 00:00:00 by hvbyteam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	skip_whitespace(const char *input, int *i)
{
	while (ft_isspace(input[*i]))
		(*i)++;
}

static char	*extract_quoted_token(const char *input, int *i)
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
	ft_strlcpy(token, &input[start], len + 1);
	return (token);
}

static char	*extract_operator_token(const char *input, int *i)
{
	int		len;
	char	*token;

	len = operator_token_length(&input[*i]);
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, &input[*i], len + 1);
	*i += len;
	return (token);
}

static char	*extract_word_token(const char *input, int *i)
{
	int		start;
	int		len;
	char	*token;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator_char(input[*i]))
		(*i)++;
	len = *i - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, &input[start], len + 1);
	return (token);
}

char	*extract_token(const char *input, int *index)
{
	int		i;
	char	*token;

	i = *index;
	skip_whitespace(input, &i);
	if (input[i] == '\0')
		return (NULL);
	if (input[i] == '\'' || input[i] == '"')
		token = extract_quoted_token(input, &i);
	else if (is_operator_char(input[i]))
		token = extract_operator_token(input, &i);
	else
		token = extract_word_token(input, &i);
	*index = i;
	return (token);
}
