#include "../include/minishell.h"
#include "../libft/libft.h"

static void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	if (cmd->heredoc_delims)
	{
		i = 0;
		while (cmd->heredoc_delims[i])
			free(cmd->heredoc_delims[i++]);
		free(cmd->heredoc_delims);
	}
	free(cmd->infile);
	free(cmd->outfile);
	free(cmd);
}

static int	is_redirect(const char *token)
{
	return (!ft_strcmp(token, "<")
		|| !ft_strcmp(token, ">")
		|| !ft_strcmp(token, ">>")
		|| !ft_strcmp(token, "<<"));
}

static void	parse_redirect(t_cmd *cmd, char **tokens, int *i)
{
	int		fd;
	char	*filename;

	if (!ft_strcmp(tokens[*i], "<"))
	{
		cmd->infile = ft_strdup(tokens[++(*i)]);
	}
	else if (!ft_strcmp(tokens[*i], ">"))
	{
		filename = ft_strdup(tokens[++(*i)]);
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			perror(filename);
		else
			close(fd);
		if (cmd->outfile)
			free(cmd->outfile);
		cmd->outfile = filename;
		cmd->append = 0;
	}
	else if (!ft_strcmp(tokens[*i], ">>"))
	{
		filename = ft_strdup(tokens[++(*i)]);
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			perror(filename);
		else
			close(fd);
		if (cmd->outfile)
			free(cmd->outfile);
		cmd->outfile = filename;
		cmd->append = 1;
	}
	else if (!ft_strcmp(tokens[*i], "<<"))
	{
		add_heredoc(cmd, tokens[++(*i)]);
		cmd->heredoc = 1;
		(*i)++;
		return ;
	}
	(*i)++;
}

t_cmd	*parser(char **tokens)
{
	t_cmd	*cmds;
	t_cmd	*current;
	int		i;
	int		cmd_start;
	int		has_heredoc;

	cmds = NULL;
	i = 0;
	while (tokens[i])
	{
		current = init_cmd();
		cmd_start = i;
		has_heredoc = 0;
		while (tokens[i] && ft_strcmp(tokens[i], "|"))
		{
			if (is_redirect(tokens[i]))
			{
				if (!ft_strcmp(tokens[i], "<<"))
					has_heredoc = 1;
				parse_redirect(current, tokens, &i);
			}
			else
				i++;
		}
		current->args = copy_args(tokens, cmd_start, i);
		if (has_heredoc || current->args)
			add_cmd(&cmds, current);
		else
			free_cmd(current);
		if (tokens[i])
			i++;
	}
	return (cmds);
}
