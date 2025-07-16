/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:19:04 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/03 00:19:06 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	ft_pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", buf);
	free(buf);
	return (0);
}
