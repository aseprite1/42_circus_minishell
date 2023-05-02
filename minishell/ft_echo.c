/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 08:51:08 by geonlee           #+#    #+#             */
/*   Updated: 2023/05/02 19:52:22 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

int	ft_strlen(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

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

void	ft_echo(char **arg)
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
}

int main(int ac, char **ag)
{
    ft_echo(ag);
}