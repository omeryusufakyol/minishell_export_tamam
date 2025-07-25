/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:39:17 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/17 00:21:06 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static void	handle_sigquit(int sig)
{
	(void)sig;
}

void	handle_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

int	main(int ac, char **av, char **envp)
{
	t_ms	ms;

	if (ac != 1)
		return (printf("Minishell does not accept arguments!\n"), 1);
	(void)ac;
	(void)av;
	rl_catch_signals = 0;
	init_minishell(&ms, envp);
	mini_loop(&ms);
	free_minishell(&ms);
	return (0);
}
