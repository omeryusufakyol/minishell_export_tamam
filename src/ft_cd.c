/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:06:41 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/03 00:06:44 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static char	*get_home(t_ms *ms)
{
	return (get_env_value(ms, "HOME"));
}

static char	*get_oldpwd(t_ms *ms)
{
	char	*oldpwd;

	oldpwd = get_env_value(ms, "OLDPWD");
	if (!*oldpwd)
	{
		free(oldpwd);
		write(2, "minishell: cd: OLDPWD not set\n", 30);
		return (NULL);
	}
	printf("%s\n", oldpwd);
	return (oldpwd);
}

static char	*get_target(char **args, t_ms *ms)
{
	if (!args[1])
		return (get_home(ms));
	if (!ft_strcmp(args[1], "--"))
	{
		if (!args[2])
			return (get_home(ms));
		else
			return (ft_strdup(args[2]));
	}
	if (!ft_strcmp(args[1], "-"))
		return (get_oldpwd(ms));
	return (ft_strdup(args[1]));
}

static void	update_pwd_env(t_ms *ms, char *oldpwd)
{
	char	*tmp;
	char	*cwd;

	tmp = ft_strjoin("OLDPWD=", oldpwd);
	update_env(ms, tmp);
	free(tmp);
	cwd = getcwd(NULL, 0);
	tmp = ft_strjoin("PWD=", cwd);
	update_env(ms, tmp);
	free(tmp);
	free(cwd);
}

int	ft_cd(char **args, t_ms *ms)
{
	char	*path;
	char	*oldpwd;

	if (cd_check_args(args, ms))
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("getcwd"), 1);
	path = get_target(args, ms);
	if (!path)
		return (free(oldpwd), 1);
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		free(path);
		return (1);
	}
	update_pwd_env(ms, oldpwd);
	free(oldpwd);
	free(path);
	return (0);
}
