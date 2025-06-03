/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mregnaut <mregnaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 23:18:03 by mregnaut          #+#    #+#             */
/*   Updated: 2025/06/04 00:04:00 by mregnaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	is_builtin(char *str)
{
	static char	*builtin_str[] = {BUILTIN_STR};
	static int	(*builtin_func[])(char **) = {BUILTIN_FUNC};
	int	i;

	i = 0;
	while (builtin_str[i])
	{
		if (ft_strcmp(str, builtin_str[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int main(int argc, char **argv)
{
	char *test_values[] = {"echo", "goinfre", "cd", "gnome", "badaboum", "", "echo", NULL};
	int i = 0;
	while (test_values[i])
	{
		__builtin_printf("test_values[%d]: %s (exist?: %s)\n", i, test_values[i], is_builtin(test_values[i]));
		i++;
	}

	return (0);
}
