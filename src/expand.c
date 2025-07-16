#include "../include/minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>

char	*get_env_value(t_ms *ms, const char *name)
{
	int		i;
	size_t	len;

	if (!name || name[0] == '\0')
		return (ft_strdup("$"));
	i = 0;
	len = ft_strlen(name);
	while (ms->env[i])
	{
		if (!ft_strncmp(ms->env[i], name, len) && ms->env[i][len] == '=')
			return (ft_strdup(&ms->env[i][len + 1]));
		i++;
	}
	return (ft_strdup(""));
}

static char	*ft_strjoin_free(char *s1, char *s2)
{
	char *joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

static int	check_tilde(const char *token, t_ms *ms, char **result)
{
	char	*home;
	char	*rest;

	if (!token || token[0] != '~')
		return (0);
	if (token[1] && token[1] != '/')
		return (0);
	home = get_env_value(ms, "HOME");
	if (!home)
		return (0);
	if (token[1] == '\0')
		*result = home;
	else
	{
		rest = ft_strdup(token + 1);
		*result = ft_strjoin_free(home, rest);
	}
	return (1);
}

static char	*expand_token(const char *token, t_ms *ms)
{
	char	*result;
	char	*temp;
	char	*val;
	size_t	i;
	size_t	start;

	if (!token)
		return (NULL);
	if (check_tilde(token, ms, &result))
		return (result);
	result = ft_strdup("");
	i = 0;
	while (token[i])
	{
		if (token[i] == '\'')
		{
			start = ++i;
			while (token[i] && token[i] != '\'')
				i++;
			temp = ft_strndup(token + start, i - start);
			result = ft_strjoin_free(result, temp);
			if (token[i])
				i++;
		}
		else if (token[i] == '"')
		{
			start = ++i;
			while (token[i] && token[i] != '"')
			{
				if (token[i] == '$' && token[i + 1] == '?')
				{
					temp = ft_itoa(ms->last_exit);
					result = ft_strjoin_free(result, temp);
					i += 2;
				}
				else if (token[i] == '$' && (ft_isalnum(token[i + 1]) || token[i + 1] == '_'))
				{
					i++;
					start = i;
					while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
						i++;
					temp = ft_strndup(token + start, i - start);
					val = get_env_value(ms, temp);
					result = ft_strjoin_free(result, val);
					free(temp);
				}
				else
				{
					result = ft_strjoin_free(result, ft_strndup(token + i, 1));
					i++;
				}
			}
			if (token[i] == '"')
				i++;
		}
		else if (token[i] == '$')
		{
			if (token[i + 1] == '?')
			{
				temp = ft_itoa(ms->last_exit);
				result = ft_strjoin_free(result, temp);
				i += 2;
			}
			else if (token[i + 1] && (ft_isalnum(token[i + 1]) || token[i + 1] == '_'))
			{
				i++;
				start = i;
				while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
					i++;
				temp = ft_strndup(token + start, i - start);
				val = get_env_value(ms, temp);
				result = ft_strjoin_free(result, val);
				free(temp);
			}
			else
			{
				result = ft_strjoin_free(result, ft_strdup("$"));
				i++;
			}
		}
		else
		{
			start = i;
			while (token[i] && token[i] != '$' && token[i] != '\'' && token[i] != '"')
				i++;
			temp = ft_strndup(token + start, i - start);
			result = ft_strjoin_free(result, temp);
		}
	}
	return (result);
}

char	**expand_tokens(char **tokens, t_ms *ms)
{
	int		i;
	char	**result;

	if (!tokens)
		return (NULL);
	i = 0;
	while (tokens[i])
		i++;
	result = malloc(sizeof(char *) * (i + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		result[i] = expand_token(tokens[i], ms);
		i++;
	}
	result[i] = NULL;
	return (result);
}
