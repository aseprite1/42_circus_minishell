/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 10:07:24 by geonlee           #+#    #+#             */
/*   Updated: 2023/05/06 09:10:50 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int ac, char **ag, char **env)
{
	int fds[2];
    pid_t pid;
    int is_pipe = 1;
    if (is_pipe == 1)
    {
        pipe(fds);
        pid = fork();
        if (pid == -1)
            return 1;
    }
}


int	exec_out_rdr_append(int fd, char **ag, char **env)
{
	int	fd_temp;
	char **test;

	fd = open(ag[4], O_APPEND | O_CREAT | O_WRONLY, 0644);
	fd_temp = dup2(fd, STDOUT_FILENO);
	if (fd_temp == -1)
		error_fd_close(fd);
	test = (char **)malloc(sizeof(char *) * 3);
	if (!test)
		error_fd_close(fd);
	test[0] = ft_strjoin("/bin/",ag[1]);
	test[1] = strdup(ag[2]);
	test[2] = NULL;
	if (execve(test[0], test, env) == -1)
		return (1);
	if (close(fd) == -1)
		return (1);
	return (0);
}
