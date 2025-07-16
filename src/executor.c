/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 23:42:24 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/17 00:21:57 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

static void	handle_outfile(t_cmd *cmd)
{
	int	fd;

	if (cmd->append)
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(cmd->outfile);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	redirect(t_cmd *cmd, t_ms *ms)
{
	int	fd;

	if (cmd->heredoc && handle_heredoc(cmd, ms))
		exit(1);
	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
		{
			perror(cmd->infile);
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->outfile)
		handle_outfile(cmd);
}

static void	run_single(t_cmd *cmds, t_ms *ms)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid == 0)
	{
		redirect(cmds, ms);
		if (is_builtin(cmds->args[0]))
			exit(run_builtin(cmds, ms));
		path = find_path(cmds->args[0], ms->env);
		if (!path)
		{
			printf("minishell: command not found: %s\n", cmds->args[0]);
			exit(127);
		}
		execve(path, cmds->args, ms->env);
		perror("execve");
		exit(1);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ms->last_exit = WEXITSTATUS(status);
}

static void	handle_heredoc_only(t_cmd *cmds, t_ms *ms)
{
	int	stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	redirect(cmds, ms);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	ms->last_exit = 0;
}

void	execute(t_cmd *cmds, t_ms *ms)
{
	if (!cmds)
		return ;
	if (!cmds->args || !cmds->args[0])
	{
		if (cmds->heredoc)
		{
			handle_heredoc_only(cmds, ms);
		}
		return ;
	}
	if (cmds->next)
	{
		run_pipeline(cmds, ms);
		return ;
	}
	if (is_builtin(cmds->args[0])
		&& !cmds->infile && !cmds->outfile && !cmds->heredoc)
	{
		ms->last_exit = run_builtin(cmds, ms);
		return ;
	}
	run_single(cmds, ms);
}
