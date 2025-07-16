/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:33:38 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/16 23:30:40 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

void	init_minishell(t_ms *ms, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	ms->env = malloc(sizeof(char *) * (i + 1));
	if (!ms->env)
		return ;
	i = 0;
	while (envp[i])
	{
		ms->env[i] = ft_strdup(envp[i]);
		i++;
	}
	ms->env[i] = NULL;
	ms->last_exit = 0;
	ms->export_only = NULL;
}
