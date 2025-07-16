/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 23:42:04 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/02 23:42:06 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char	*join_path(const char *dir, const char *cmd)
{
	char	*full;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full = malloc(len);
	if (!full)
		return (NULL);
	i = 0;
	while (dir[i])
	{
		full[i] = dir[i];
		i++;
	}
	full[i++] = '/';
	j = 0;
	while (cmd[j])
		full[i++] = cmd[j++];
	full[i] = '\0';
	return (full);
}

char	*find_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	while (*env && ft_strncmp(*env, "PATH=", 5))
		env++;
	if (!*env)
		return (NULL);
	paths = ft_split(*env + 5, ':');
	i = 0;
	while (paths[i])
	{
		path = join_path(paths[i], cmd);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}
