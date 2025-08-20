/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvbyteam <hvbyteam@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:00:00 by hvbyteam          #+#    #+#             */
/*   Updated: 2025/07/31 00:00:00 by hvbyteam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	is_redirect_type(int type)
{
	return (type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_IN
		|| type == TOKEN_REDIRECT_APPEND
		|| type == TOKEN_HEREDOC);
}

void	refine_token_types(t_token *head)
{
	t_token	*tmp;
	int		command_found;

	tmp = head;
	command_found = 0;
	while (tmp)
	{
		if (tmp->type == TOKEN_PIPE)
			command_found = 0;
		if (tmp->type == TOKEN_OTHER)
		{
			if (!command_found)
			{
				tmp->type = TOKEN_COMMAND;
				command_found = 1;
			}
			else if (tmp->prev && is_redirect_type(tmp->prev->type))
				tmp->type = TOKEN_FILE;
			else
				tmp->type = TOKEN_ARGUMENT;
		}
		tmp = tmp->next;
	}
}

t_token	*line_lexer(const char *line)
{
	int				index;
	char			*value;
	t_token			*head;
	t_token			*token;
	t_quote_type	quoted;

	index = 0;
	head = NULL;
	value = extract_token(line, &index);
	while (value != NULL)
	{
		quoted = get_quote_type(value);
		token = new_token(value, quoted);
		free(value);
		if (!token)
		{
			free_tokens(head);
			return (NULL);
		}
		append_token(&head, token);
		value = extract_token(line, &index);
	}
	refine_token_types(head);
	return (head);
}
