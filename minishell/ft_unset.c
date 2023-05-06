/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 06:35:42 by jaeyan            #+#    #+#             */
/*   Updated: 2023/05/06 09:10:48 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	unset_arg_checker(char *str)
{
	int	i;

	i = 0;
	if (env_rule_check(str[i], 1))
		return (1);
	while (str[i])
	{
		if (env_rule_check(str[i], 0))
			return (1);		
		++i;
	}
	return (0);
}

void	env_del(char *find, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (env_cmp(env[i], find))
		{
			free (env[i]);
			while (env[i])
			{
				env[i] = env[i + 1];
				++i;
			}
			return ;
		}
		++i;
	}
	return ;
}

void	ft_unset(char **arg, char **env)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		if (unset_arg_checker(arg[i]))
			printf("-minishell: unset: `%s': not a valid identifier\n", arg[i]);
		else
			env_del(arg[i], env);
		++i;
	}
}
