/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/08/18 19:28:29 by Hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	process_argument_token(t_cmd *current, t_token *tokens)
{
	char	*expanded;
	char	*removed;

	if (tokens->quote_type != SINGLE_QUOTE)
	{
		expanded = expand_variables(tokens->value);
		free(tokens->value);
		tokens->value = expanded;
		if (!tokens->value)
			return (0);
	}
	if (tokens->quote_type != NO_QUOTE)
	{
		removed = remove_quotes(tokens->value);
		free(tokens->value);
		tokens->value = removed;
	}
	/* ignorer argument vide issu d'une expansion non quotee */
	if (!(tokens->value[0] == '\0' && tokens->quote_type == NO_QUOTE)
		&& !(tokens->value[0] == '$' && tokens->value[1] == '\0'
			&& tokens->prev && tokens->prev->quote_type != NO_QUOTE))
	{
		if (!add_arg(current, tokens->value))
			return (0);
	}
	return (1);
}

static void	handle_pipe_token(t_cmd **head, t_cmd **current)
{
	append_command(head, *current);
	*current = NULL;
}

static t_cmd	*process_token(t_cmd **head, t_cmd **current, t_token **tokens)
{
	if (!*current)
	{
		*current = new_command();
		if (!*current)
			return (NULL);
	}
	if ((*tokens)->type == TOKEN_PIPE)
	{
		handle_pipe_token(head, current);
		return ((t_cmd *)1);
	}
	else if (is_argument_type((*tokens)->type))
	{
		if (!process_argument_token(*current, *tokens))
			return (NULL);
	}
	else if (is_redirection((*tokens)->type))
	{
		if (!handle_redirection(*current, *tokens))
			return (NULL);
		*tokens = (*tokens)->next;
	}
	return (*current);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (!process_token(&head, &current, &tokens))
		{
			if (current)
				free_commands(current);
			if (head)
				free_commands(head);
			return (NULL);
		}
		tokens = tokens->next;
	}
	if (current)
		append_command(&head, current);
	return (head);
}
