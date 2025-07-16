/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:13 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/03 00:07:15 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <unistd.h>
#include <stdlib.h>

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args, t_ms *ms)
{
	int	code;

	write(1, "exit\n", 5);
	if (!args[1])
		exit(ms->last_exit);
	if (!is_numeric(args[1]))
	{
		write(2, "minishell: exit: numeric argument required\n", 43);
		exit(255);
	}
	if (args[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	code = ft_atoi(args[1]);
	exit (code);
}
