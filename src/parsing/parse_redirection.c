/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:45:00 by Hadia             #+#    #+#             */
/*   Updated: 2025/08/18 19:39:51 by Hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*process_filename(char *filename, t_quote_type quote_type)
{
	char	*expanded;
	char	*removed;

	expanded = NULL;
	if (quote_type != SINGLE_QUOTE)
	{
		expanded = expand_variables(filename);
		if (!expanded)
			return (NULL);
		filename = expanded;
	}
	if (quote_type != NO_QUOTE)
	{
		removed = remove_quotes(filename);
		if (expanded)
			free(expanded);
		if (!removed)
			return (NULL);
		filename = removed;
	}
	return (filename);
}

static void	set_input_redirection(t_cmd *current, t_token_type type,
	char *filename)
{
	if (type == TOKEN_REDIRECT_IN)
	{
		if (current->input_file)
			free(current->input_file);
		current->input_file = ft_strdup(filename);
	}
	else if (type == TOKEN_HEREDOC)
	{
		if (current->input_file)
			free(current->input_file);
		current->input_file = ft_strdup(filename);
		current->heredoc = 1;
	}
}

static void	set_output_redirection(t_cmd *current, t_token_type type,
	char *filename)
{
	if (type == TOKEN_REDIRECT_OUT)
	{
		if (current->output_file)
			free(current->output_file);
		current->output_file = ft_strdup(filename);
		current->append_output = 0;
	}
	else if (type == TOKEN_REDIRECT_APPEND)
	{
		if (current->output_file)
			free(current->output_file);
		current->output_file = ft_strdup(filename);
		current->append_output = 1;
	}
}

static void	set_redirection_file(t_cmd *current, t_token_type type,
	char *filename)
{
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_HEREDOC)
		set_input_redirection(current, type, filename);
	else
		set_output_redirection(current, type, filename);
}

int	handle_redirection(t_cmd *current, t_token *tokens)
{
	char	*filename;
	char	*processed_filename;
	int		should_free;

	if (!tokens->next || tokens->next->type != TOKEN_FILE)
		return (0);
	filename = tokens->next->value;
	processed_filename = process_filename(filename, tokens->next->quote_type);
	if (!processed_filename)
		return (0);
	set_redirection_file(current, tokens->type, processed_filename);
	should_free = (tokens->next->quote_type != NO_QUOTE
			|| (tokens->next->quote_type != SINGLE_QUOTE
				&& processed_filename != filename));
	if (should_free)
		free(processed_filename);
	return (1);
}
