/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mattm <mattm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/08/07 17:00:24 by mattm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

extern char	**environ;

static int	count_env_vars(void)
{
	int	count;

	count = 0;
	while (environ[count])
		count++;
	return (count);
}

static char	*create_env_string(const char *key, const char *value)
{
	char	*env_str;
	size_t	key_len;
	size_t	val_len;
	size_t	total_len;

	key_len = ft_strlen(key);
	val_len = ft_strlen(value);
	total_len = key_len + val_len + 2;
	env_str = malloc(total_len);
	if (!env_str)
		return (NULL);
	ft_strlcpy(env_str, key, key_len + 1);
	ft_strlcat(env_str, "=", total_len);
	ft_strlcat(env_str, value, total_len);
	return (env_str);
}

static int	find_env_var(const char *key)
{
	int		i;
	size_t	key_len;

	key_len = ft_strlen(key);
	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], key, key_len) == 0
			&& environ[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	ft_setenv(const char *key, const char *value, int overwrite)
{
	int		index;
	char	**new_environ;
	char	*new_var;
	int		count;

	if (!key || !value || ft_strchr(key, '='))
		return (-1);
	index = find_env_var(key);
	if (index != -1)
	{
		if (!overwrite)
			return (0);
		free(environ[index]);
		environ[index] = create_env_string(key, value);
		if (environ[index])
			return (0);
		else
			return (-1);
	}
	count = count_env_vars();
	new_environ = malloc(sizeof(char *) * (count + 2));
	if (!new_environ)
		return (-1);
	new_var = create_env_string(key, value);
	if (!new_var)
	{
		free(new_environ);
		return (-1);
	}
	ft_memcpy(new_environ, environ, sizeof(char *) * count);
	new_environ[count] = new_var;
	new_environ[count + 1] = NULL;
	environ = new_environ;
	return (0);
}

int	ft_unsetenv(const char *key)
{
	int	index;
	int	i;

	if (!key || ft_strchr(key, '='))
		return (-1);
	index = find_env_var(key);
	if (index == -1)
		return (0);
	free(environ[index]);
	i = index;
	while (environ[i])
	{
		environ[i] = environ[i + 1];
		i++;
	}
	return (0);
}
