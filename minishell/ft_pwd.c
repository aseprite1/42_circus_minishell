/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 06:20:31 by geonlee           #+#    #+#             */
/*   Updated: 2023/05/06 09:10:47 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	ft_pwd(char **env, int pipe_len)
{
	char	*pwd;

	pwd = get_env("PWD", env);
    if (pwd==NULL)
    {
        if (pipe_len)
			exit(1);
    }
	else
	{
		printf("%s\n",pwd);
		if (pipe_len)
			exit(0);
		//exitcode 0
	}
}