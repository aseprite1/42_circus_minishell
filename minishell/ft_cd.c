/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 04:30:26 by geonlee           #+#    #+#             */
/*   Updated: 2023/05/02 19:04:30 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void    ft_cd_home()
{
    char    *pwd;
    char    *home;

    getcwd()
}

void	ft_cd_home(int pipe_cnt)
{
	char	*pwd;
	char	*home;
}

void    ft_cd(char **str)
{
    if (!str[1]) // 그냥 cd일때
		ft_cd_home(pipe_cnt);
    chdir();
}