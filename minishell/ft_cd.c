/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 06:18:17 by geonlee           #+#    #+#             */
/*   Updated: 2023/05/06 07:47:22 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	update_oldpwd(char **env, char *pwd)
{
	int n;
	char *tmp;

	n  = 0;
	if (!pwd)
		return ;
	while (env[n])
	{
		if (env_cmp(env[n], "OLDPWD"))
		{
			tmp = env[n];
			env[n] = ft_strjoin("OLDPWD=",pwd);
			free(tmp);
			return ;
		}
	n++;
	}
}

void	update_pwd(char **env, char *pwd)
{
	int n;
	char *tmp;

	n  = 0;
	if (!pwd)
		return ;
	while (env[n])
	{
		if (env_cmp(env[n], "PWD"))
		{
			tmp = env[n];
			env[n] = ft_strjoin("PWD=",pwd);
			free(tmp);
			return ;
		}
    n++;
	}
}

void	cd_home(char **env, int pipe_len)
{
	char	*home;
	char	*pwd;

	pwd = get_env("PWD", env);
	home = get_env("HOME", env);
	if (home == NULL)
	{
		printf("-minishell: cd: HOME not set\n");
    //exit code 1
		if (pipe_len > 0)
			exit(1);
	}
	else
	{
		chdir(home);
		update_pwd(env, home);
		update_oldpwd(env, pwd);
    //exit code 0
		if (pipe_len > 0)
			exit(1);
	}
}

void	ft_cd(char **arg, char **env, int pipe_len)
{
	int		chdir_check;
	char	*home;
	char	*cwd;
	char	*pwd;

	if (!arg[1])
		cd_home(env, pipe_len);
	else
	{
		cwd = getcwd(NULL, 0);
		pwd = get_env("PWD", env);
		home = get_env("HOME", env);
		if (arg[1][0] == '~')
		{
			chdir_check = chdir("/");		
			chdir_check = chdir(home);
			if (arg[1][1] && arg[1][1] == '/' && arg[1][2])
				chdir_check = chdir(&arg[1][2]);
			else if (arg[1][1] && arg[1][1] != '/')
				chdir_check = -1;
			if (chdir_check == -1)
				printf("minishell : cd: %s: No such file or directory\n", &arg[1][1]);
		}
		else
		{
			chdir_check = chdir(arg[1]);
			if (chdir_check == -1)
				printf("minishell: cd: %s: No such file or directory\n", arg[1]);
		}
		if (chdir_check  == 0)
		{
			free(cwd);
			cwd = getcwd(NULL, 0);
			update_pwd(env, cwd);
			update_oldpwd(env, pwd);
			//exitcode 0
			if (pipe_len)
				exit(0);
		}
		else
		{
			chdir(cwd);
			//exitcode 1 
			if (pipe_len)
				exit(1);
		}
	}
}
