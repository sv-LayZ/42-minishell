/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mattm <mattm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 21:35:47 by mregnaut          #+#    #+#             */
/*   Updated: 2025/08/07 17:09:09 by mattm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

# define BUILTIN_CMDS "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
# define BUILTIN_FUNCS builtin_echo, builtin_cd, builtin_pwd, builtin_export, builtin_unset, builtin_env, builtin_exit, NULL

/* Builtin function declarations */
int	builtin_echo(char **args);
int	builtin_cd(char **args);
int	builtin_pwd(char **args);
int	builtin_export(char **args);
int	builtin_unset(char **args);
int	builtin_env(char **args);
int	builtin_exit(char **args);

/* Utility functions */
int	is_builtin(char *cmd);
int	execute_builtin(int index, char **args);

/* Environment manipulation functions */
int	ft_setenv(const char *key, const char *value, int overwrite);
int	ft_unsetenv(const char *key);

#endif
