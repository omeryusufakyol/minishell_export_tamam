/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:18:28 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/16 23:48:58 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

static int	env_len(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

static int	key_in_env(char **env, const char *key)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(key);
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
			return (1);
		i++;
	}
	return (0);
}

static char	**combine_env_export(char **env, char **export_only)
{
	int		i;
	int		total;
	char	**combined;
	char	*key;

	total = env_len(env) + env_len(export_only);
	combined = malloc(sizeof(char *) * (total + 1));
	if (!combined)
		return (NULL);
	i = 0;
	if (env)
	{
		while (*env)
			combined[i++] = ft_strdup(*env++);
	}
	if (export_only)
	{
		while (*export_only)
		{
			key = *export_only;
			if (!key_in_env(combined, key))
				combined[i++] = ft_strdup(key);
			export_only++;
		}
	}
	combined[i] = NULL;
	return (combined);
}

static void	sort_env_copy(char **copy, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < len - 1)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strcmp(copy[i], copy[j]) > 0)
			{
				tmp = copy[i];
				copy[i] = copy[j];
				copy[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_export(t_ms *ms)
{
	char	**all;
	int		len;
	int		i;
	char	*eq;

	all = combine_env_export(ms->env, ms->export_only);
	if (!all)
		return ;
	len = env_len(all);
	sort_env_copy(all, len);
	i = 0;
	while (i < len)
	{
		eq = ft_strchr(all[i], '=');
		if (eq)
		{
			*eq = '\0';
			printf("declare -x %s=\"%s\"\n", all[i], eq + 1);
			*eq = '=';
		}
		else
			printf("declare -x %s\n", all[i]);
		free(all[i]);
		i++;
	}
	free(all);
}
