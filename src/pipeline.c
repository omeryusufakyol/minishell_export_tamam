/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:46:57 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/17 00:22:04 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

static int	wait_all(pid_t *pids, int count)
{
	int	status;
	int	last;
	int	i;

	last = 0;
	i = count;
	while (i--)
	{
		waitpid(pids[i], &status, 0);
		if (i == 0)
			last = status;
	}
	if (WIFEXITED(last))
		return (WEXITSTATUS(last));
	return (1);
}

static int	launch_process(t_cmd *cmd, t_ms *ms, int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		dup2(in_fd, STDIN_FILENO);
		dup2(out_fd, STDOUT_FILENO);
		redirect(cmd, ms);
		if (is_builtin(cmd->args[0]))
			exit(run_builtin(cmd, ms));
		execve(find_path(cmd->args[0], ms->env), cmd->args, ms->env);
		perror("execve");
		exit(1);
	}
	return (pid);
}

static int	run_pipeline_loop(t_cmd **cmds, t_ms *ms, int *in_fd, pid_t *pids)
{
	int	p[2];
	int	i;

	i = 0;
	while (*cmds && (*cmds)->next)
	{
		if (i >= MAX_CMDS)
		{
			write(2, "minishell: too many piped commands\n", 35);
			return (-1);
		}
		if (pipe(p) == -1)
			return (perror("pipe"), -1);
		pids[i++] = launch_process(*cmds, ms, *in_fd, p[1]);
		close(p[1]);
		if (*in_fd != STDIN_FILENO)
			close(*in_fd);
		*in_fd = p[0];
		*cmds = (*cmds)->next;
	}
	return (i);
}

int	run_pipeline(t_cmd *cmds, t_ms *ms)
{
	int		in_fd;
	pid_t	pids[MAX_CMDS];
	int		i;

	in_fd = STDIN_FILENO;
	i = run_pipeline_loop(&cmds, ms, &in_fd, pids);
	if (i == -1)
		return (1);
	pids[i++] = launch_process(cmds, ms, in_fd, STDOUT_FILENO);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	return (ms->last_exit = wait_all(pids, i));
}
