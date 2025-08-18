/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvbyteam <hvbyteam@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:00:00 by hvbyteam          #+#    #+#             */
/*   Updated: 2025/07/31 00:00:00 by hvbyteam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token_type	get_token_type(const char *value)
{
	if (!value)
		return (TOKEN_OTHER);
	if (ft_strcmp(value, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(value, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	else if (ft_strcmp(value, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	else if (ft_strcmp(value, ">>") == 0)
		return (TOKEN_REDIRECT_APPEND);
	else if (ft_strcmp(value, "<<") == 0)
		return (TOKEN_HEREDOC);
	else if (value[0] == '$')
		return (TOKEN_ENV_VAR);
	return (TOKEN_OTHER);
}
