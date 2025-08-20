/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <hadia@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 22:11:37 by Hadia             #+#    #+#             */
/*   Updated: 2025/08/20 17:11:52 by hadia            ###   ########.fr       */
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
