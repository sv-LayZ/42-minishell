/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvbyteam <hvbyteam@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:00:00 by hvbyteam          #+#    #+#             */
/*   Updated: 2025/07/31 00:00:00 by hvbyteam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_argument_type(t_token_type type)
{
	return (type == TOKEN_COMMAND
		|| type == TOKEN_ARGUMENT || type == TOKEN_ENV_VAR);
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_IN
		|| type == TOKEN_REDIRECT_APPEND
		|| type == TOKEN_HEREDOC);
}
