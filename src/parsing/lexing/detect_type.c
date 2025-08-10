/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvbyteam <hvbyteam@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:00:00 by hvbyteam          #+#    #+#             */
/*   Updated: 2025/08/06 00:00:00 by hvbyteam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	skip_whitespace(const char *input, int *i)
{
	while (ft_isspace(input[*i]))
		(*i)++;
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
