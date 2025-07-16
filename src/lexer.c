/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:38:55 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/03 00:38:56 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

static char	*extract_token(const char *s, int *i)
{
	int		len;
	int		start;
	char	*token;

	while (s[*i] == ' ')
		(*i)++;
	start = *i;
	len = token_len(s, *i);
	*i += len;
	token = ft_strndup(&s[start], len);
	if (!token)
		return (NULL);
	return (token);
}

static void	free_tokens(char **tokens, int j)
{
	while (j-- > 0)
		free(tokens[j]);
	free(tokens);
}

char	**lexer(const char *input)
{
	int		count;
	int		i;
	int		j;
	char	**tokens;

	count = count_tokens(input);
	tokens = malloc(sizeof(char *) * (count + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	j = 0;
	while (j < count)
	{
		tokens[j] = extract_token(input, &i);
		if (!tokens[j])
			return (free_tokens(tokens, j), NULL);
		j++;
	}
	tokens[j] = NULL;
	return (tokens);
}
