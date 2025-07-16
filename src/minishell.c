#include "../include/minishell.h"
#include "../libft/libft.h"

static void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		if (cmd->heredoc_delims)
		{
			i = 0;
			while (cmd->heredoc_delims[i])
			{
				free(cmd->heredoc_delims[i]);
				i++;
			}
			free(cmd->heredoc_delims);
		}
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd);
		cmd = tmp;
	}
}

static void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

static void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static void	process_multiline_input(char *input, t_ms *ms)
{
	char	**lines;
	char	**tokens;
	char	**expanded;
	t_cmd	*cmds;
	int		i;

	lines = ft_split(input, '\n');
	i = 0;
	while (lines[i])
	{
		if (*lines[i])
		{
			tokens = lexer(lines[i]);
			if (!check_syntax(tokens, ms))
			{
				expanded = expand_tokens(tokens, ms);
				if (!check_syntax(expanded, ms))
				{
					cmds = parser(expanded);
					execute(cmds, ms);
					free_cmds(cmds);
				}
				free_tokens(expanded);
			}
			free_tokens(tokens);
		}
		i++;
	}
	free_split(lines);
}

void	mini_loop(t_ms *ms)
{
	char	*input;
	char	**tokens;
	char	**expanded;
	t_cmd	*cmds;

	while (1)
	{
		handle_signals();
		input = readline("minishell$ ");
		if (!input)
		{
			write(1, "\nexit\n", 6);
			break ;
		}
		if (*input)
			add_history(input);
		ms->raw_input = input;
		if (ft_strchr(input, '\n'))
		{
			process_multiline_input(input, ms);
			free(input);
			continue ;
		}
		tokens = lexer(input);
		if (check_syntax(tokens, ms))
		{
			free(input);
			free_tokens(tokens);
			continue ;
		}
		expanded = expand_tokens(tokens, ms);
		if (check_syntax(expanded, ms))
		{
			free(input);
			free_tokens(tokens);
			free_tokens(expanded);
			continue ;
		}
		cmds = parser(expanded);
		execute(cmds, ms);
		//print_cmds(cmds);
		free(input);
		free_tokens(tokens);
		free_tokens(expanded);
		free_cmds(cmds);
	}
	(void)ms;
}
