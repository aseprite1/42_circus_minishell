/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 08:51:08 by geonlee           #+#    #+#             */
/*   Updated: 2023/05/06 06:18:21 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	is_echo_flag(char *str)
{
	if (*str != '-')
		return (0);
	++str;
	while (*str)
	{
		if (*str != 'n')
			return (0);
		++str;
	}
	return (1);
}

void	ft_echo(char **arg, int pipe_len)
{
	int		idx;
	int		new_line;

	new_line = 0;
	idx = 1;
	while (arg[idx] && is_echo_flag(arg[idx]))
		++idx;
	if (idx > 1)
		new_line = 1;
	while (arg[idx])
	{
		write(1, arg[idx], ft_strlen(arg[idx]));
		++idx;
		if (arg[idx])
			write(1, " ", 1);
	}
	if (!new_line)
		write(1, "\n", 1);
	if (pipe_len > 0)
		exit(0);
		//set exit 0
}
