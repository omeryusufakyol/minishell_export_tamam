/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:10:46 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/16 23:18:07 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <unistd.h>
#include <stdlib.h>

static int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || !(s[0]) || (s[0] >= '0' && s[0] <= '9'))
		return (0);
	i = 0;
	while (s[i] && s[i] != '=')
	{
		if (!(s[i] == '-' || (s[i] >= 'A' && s[i] <= 'Z')
				|| (s[i] >= 'a' && s[i] <= 'z')
				|| (s[i] >= '0' && s[i] <= '9')))
			return (0);
		i++;
	}
	return (1);
}

static char	*remove_quotes(const char *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if ((s[0] == '\'' || s[0] == '\"') && s[len - 1] == s[0])
		return (ft_strndup(s + 1, len - 2));
	return (ft_strdup(s));
}

static char	*clean_export_arg(const char *arg)
{
	char	*eq;
	char	*key;
	char	*val;
	char	*clean;
	char	*tmp;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (ft_strdup(arg));
	key = ft_strndup(arg, eq - arg);
	val = ft_strdup(eq + 1);
	clean = remove_quotes(val);
	tmp = ft_strjoin(key, "=");
	free(key);
	free(val);
	key = ft_strjoin(tmp, clean);
	free(tmp);
	free(clean);
	return (key);
}

void	update_env(t_ms *ms, const char *arg)
{
	int		i;
	size_t	len;
	char	**new_env;
	char	*clean;

	clean = clean_export_arg(arg);
	len = 0;
	while (ms->env[len])
		len++;
	if (update_if_exists(ms, clean))
		return ;
	new_env = malloc(sizeof(char *) * (len + 2));
	i = -1;
	while (++i < (int)len)
		new_env[i] = ms->env[i];
	new_env[i] = clean;
	new_env[i + 1] = NULL;
	free(ms->env);
	ms->env = new_env;
}

int	ft_export(char **args, t_ms *ms)
{
	int		i;
	char	*key;

	if (!args[1])
	{
		print_export(ms);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			write(2, "minishell: export: not a valid identifier\n", 43);
			ms->last_exit = 1;
			return (1);
		}
		if (has_equal(args[i]))
		{
			key = ft_strndup(args[i], ft_strchr(args[i], '=') - args[i]);
			remove_from_export_only(ms, key);
			free(key);
			update_env(ms, args[i]);
		}
		else
			add_to_export_only(ms, args[i]);
		i++;
	}
	return (0);
}
