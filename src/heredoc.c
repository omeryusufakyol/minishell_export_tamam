/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:29:53 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/09 22:55:37 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <stdio.h>

void	add_heredoc(t_cmd *cmd, char *delim)
{
	int		i;
	int		j;
	char	**new;

	i = 0;
	while (cmd->heredoc_delims && cmd->heredoc_delims[i])
		i++;
	new = malloc(sizeof(char *) * (i + 2));
	if (!new)
		return ;
	j = 0;
	while (j < i)
	{
		new[j] = cmd->heredoc_delims[j];
		j++;
	}
	new[i] = ft_strdup(delim);
	new[i + 1] = NULL;
	free(cmd->heredoc_delims);
	cmd->heredoc_delims = new;
}

static int	is_quoted_delim(t_ms *ms)
{
	char	*input;
	int		i;
	int		len;

	i = 0;
	input = ms->raw_input;
	len = ft_strlen(input);
	while (input[i] && !(input[i] == '<' && input[i + 1] == '<'))
		i++;
	i += 2;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if (input[i] == '\'' || input[i] == '"')
		return (1);
	return (0);
}

static void	read_heredoc_loop(int write_fd, char *delim, t_ms *ms)
{
	char	*line;
	char	*expanded;
	int		quoted;

	quoted = is_quoted_delim(ms);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || !ft_strcmp(line, delim))
		{
			free(line);
			break ;
		}
		if (quoted)
			expanded = ft_strdup(line);
		else
			expanded = expand_heredoc_line_envonly(line);
		write(write_fd, expanded, ft_strlen(expanded));
		write(write_fd, "\n", 1);
		free(expanded);
		free(line);
	}
}

int	handle_heredoc(t_cmd *cmd, t_ms *ms)
{
	int		fd[2];
	int		i;

	i = 0;
	while (cmd->heredoc_delims && cmd->heredoc_delims[i])
	{
		if (pipe(fd) == -1)
			return (perror("pipe"), 1);
		read_heredoc_loop(fd[1], cmd->heredoc_delims[i], ms);
		close(fd[1]);
		if (!cmd->heredoc_delims[i + 1])
		{
			dup2(fd[0], STDIN_FILENO);
		}
		else
			close(fd[0]);
		i++;
	}
	return (0);
}
