/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 06:22:55 by geonlee           #+#    #+#             */
/*   Updated: 2023/05/06 06:36:59 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while ((8 < *str && *str < 14) || (*str == 32))
		str++;
	if (45 == *str || 43 == *str)
	{
		if (*str == 45)
			sign = sign * -1;
		str++;
	}
	while (47 < *str && *str < 58)
	{
		result = (result * 10) + *str - '0';
		str++;
	}
	return (result * sign);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}

void	error_exit(char *arg, int pipe_len)
{
	if (!pipe_len)
		printf("exit\n");
    if (arg)
    {
        printf("minishell: exit: %s: numeric argument required\n", arg); // 나가짐 
        exit(255);
    }
    else
    {
        // exit code 1 
        printf("minishell: exit: too many arguments\n");
    }
}

void	ft_exit(char **arg, int pipe_len)
{
	int	i;

	if (arg[1])
	{
		i = 0;
		if (arg[1] && (arg[1][0] == '-' || arg[1][0] == '+'))
			++i;
		while (arg[1][i] && ft_isdigit(arg[1][i]))
			++i;
		if (i < ft_strlen(arg[1]))
			error_exit(arg[1], pipe_len);
		if (arg[2])
			error_exit(NULL, pipe_len);
		else
		{
            if (!pipe_len)
		        printf("exit\n");
            exit(ft_atoi(arg[1]));
		}
	}
	else
	{
        if (!pipe_len)
		    printf("exit\n");
        exit(0);
    }
}