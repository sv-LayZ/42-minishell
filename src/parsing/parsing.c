/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Hadia <Hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 22:11:37 by Hadia             #+#    #+#             */
/*   Updated: 2025/07/31 22:21:13 by Hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*parsing(const char *line)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = line_lexer(line);
	if (!tokens)
		return (NULL);
	cmds = parse_tokens(tokens);
	free_tokens(tokens);
	return (cmds);
}
