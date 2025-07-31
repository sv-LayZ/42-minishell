/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/07/31 22:22:07 by Hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_redirection(t_cmd *current, t_token *tokens)
{
	if (!tokens->next || tokens->next->type != TOKEN_FILE)
		return (0);
	if (tokens->type == TOKEN_REDIRECT_IN)
		current->input_file = strdup(tokens->next->value);
	else if (tokens->type == TOKEN_REDIRECT_OUT)
		current->output_file = strdup(tokens->next->value);
	else if (tokens->type == TOKEN_REDIRECT_APPEND)
	{
		current->output_file = strdup(tokens->next->value);
		current->append_output = 1;
	}
	else if (tokens->type == TOKEN_HEREDOC)
	{
		current->input_file = strdup(tokens->next->value);
		current->heredoc = 1;
	}
	return (1);
}

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
	if (!add_arg(current, tokens->value))
		return (0);
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
			return (NULL);
		tokens = tokens->next;
	}
	if (current)
		append_command(&head, current);
	return (head);
}
