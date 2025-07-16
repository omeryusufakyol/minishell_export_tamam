/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:02:03 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/03 00:02:04 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_minishell(t_ms *ms)
{
	int	i;

	i = 0;
	if (ms->env && ms->env[i])
	{
		while (ms->env[i])
		{
			free(ms->env[i]);
			i++;
		}
		free(ms->env);
	}
	else if (ms->env)
		free(ms->env);
	return ;
}
