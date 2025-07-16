/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:22:34 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/03 00:24:07 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <stdlib.h>

static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

static void	append_env_or_text(char **res, char *line, int *i)
{
	char	*temp;
	char	*val;
	int		start;

	if (line[*i] == '$' && (ft_isalpha(line[*i + 1]) || line[*i + 1] == '_'))
	{
		(*i)++;
		start = *i;
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
			(*i)++;
		temp = ft_strndup(&line[start], *i - start);
		val = getenv(temp);
		free(temp);
		if (!val)
			val = "";
		*res = ft_strjoin_free(*res, ft_strdup(val));
	}
	else
	{
		start = *i;
		while (line[*i] && line[*i] != '$')
			(*i)++;
		*res = ft_strjoin_free(*res, ft_strndup(&line[start], *i - start));
	}
}

char	*expand_heredoc_line_envonly(char *line)
{
	char	*result;
	int		i;

	if (!line)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		append_env_or_text(&result, line, &i);
	}
	return (result);
}
