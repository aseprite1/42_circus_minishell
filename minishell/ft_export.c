/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 06:33:44 by jaeyan            #+#    #+#             */
/*   Updated: 2023/05/06 09:20:29 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

char	*export_env_cmp(char *one, char *two)
{
	int		i;
	char	*rt;

	i = 0;
	while (two[i] && two[i] != '=')
	{
		if (one[i] != two[i])
			return (0);
		++i;
	}
	if (one[i] != '=')
		return (0);
	rt = one;
	return (rt);
}

int	check_eq(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (0);
		++i;
	}
	return (1);
}

void	get_env_add(char *find, char **env)
{
	int	i;

	i = 0;
	if (check_eq)
		return (0);
	while (env[i])
	{
		if (export_env_cmp(env[i], find))
		{
			free (env[i]);
			env[i] = ft_strdup(find);
			return (0);
		}
		++i;
	}
	env[i] = ft_strdup(find);
	env[i + 1] = NULL;
	return (0);
}

void	declare_x(char *str)
{
	int	i;

	i = 0;
	write(1, "declare -x ", 11);
	while (str[i] || str[i] != '=')
	{
		write(1, str[i], 1);
		++i;
	}
	write(1, str[i++], 1);
	write(1, "\"", 1);
	while (str[i])
	{
		write(1, str[i], 1);
		++i;
	}
	write(1, "\"\n", 2);
}

void	exp_only(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		declare_x(env[i]);
		++i;
	}
}

void    free_env(char **env)
{
    int i;

    i = 0;
    while (env[i])
    {
        free (env[i]);
        env[i] = NULL;
        ++i;   
    }
    free(env);
    env = NULL;
}

// void	exp_env_ch(, int i, int len, char **env)
// {
		// rt->env = envp;
		// rt->env_add = &(rt->env);
		// rt->idx = i;
		// rt->command_len = pac.pipe;


// }

void	ft_export(t_command **cmd, char **arg, int idx)
{
	int		i;
    int     j;
	char	**add;

	i = 1;
	if (arg[1])
	{
		while (arg[i])
		{
			if (export_arg_checker(arg[i]))
				printf("-minishell: export: `%s': not a valid identifier\n", arg[i]);
			else
			{
                j = 0;
				get_env_add(arg[i], cmd[idx]->env);
				add = cpy_env(cmd[idx]->env);
				free_env(cmd[idx]->env);
                while (j < cmd[0]->command_len)
                {
                    cmd[j]->env = add;
                    ++j;
                }
			}
			++i;
		}
	}
	else
		exp_only(cmd[idx]->env);
}
