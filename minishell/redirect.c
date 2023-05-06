#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	int		n;
	int		k;

	n = 0;
	result = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!result)
		return (NULL);
	result[ft_strlen(s1) + ft_strlen(s2)] = '\0';
	while (s1[n])
	{
		result[n] = s1[n];
		n++;
	}
	k = n;
	n = 0;
	while (s2[n])
	{
		result[k + n] = s2[n];
		n++;
	}
	return (result);
}

int error_fd_close(int fd)
{
	close(fd);
	return (1);
}
// O_WRONLY | O_CREAT | O_TRUNC | O_SYNC,
// 				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
int exec_out_rdr_replace(int fd, char **ag, char **env)
{
	int	fd_temp;
	char **test;

	fd = open(ag[4], O_TRUNC | O_CREAT | O_WRONLY, 0644);
	fd_temp = dup2(fd, STDOUT_FILENO);
	if (fd_temp == -1)
		error_fd_close(fd);
	test = (char **)malloc(sizeof(char *) * 3);
	if (!test)
		error_fd_close(fd);
	test[0] = ft_strjoin("/bin/",ag[1]);
	test[1] = strdup(ag[2]);
	test[2] = NULL;
	if (close(fd) == -1)
		return (1);
	if (execve(test[0], test, env) == -1)
		return (1);
	return (0);
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

int	exec_in_rdr(int fd, char **ag, char **env)
{
	int	fd_temp;
	int	bytes;
	char **test;

	fd = open(ag[3], O_RDONLY, 0644);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (error_fd_close(fd));
	test = (char **)malloc(sizeof(char *) * 3);
	if (!test)
		return (error_fd_close(fd));
	test[0] = ft_strjoin("/bin/", ag[1]);
	test[1] = strdup(ag[3]);
	test[2] = NULL;
	if (execve(test[0], test, env) == -1)
		return (1);
	if (close(fd) == -1)
		return (1);
	return 0;
}

int exec_in_heredoc(int fd, char **ag, char **env)
{
	char	*str;
	char 	**test;
	char	buffer[1];

	fd = open("heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
    while (1)
    {
        str = readline("> ");
		if (str == NULL)
		{
			if (close(fd) == -1)
				return (1);
           return 1;
		}
		if (!strcmp(str, ag[3]))
		{
			free(str);
			if (close(fd) == -1)
				return (1);
			fd = open("heredoc", O_RDONLY , 0644);
			dup2(fd, STDIN_FILENO);
			if (close(fd) == -1)
				return (1);
			test = (char **)malloc(sizeof(char *) * 3);
			if (!test)
				return (error_fd_close(fd));
			test[0] = ft_strjoin("/bin/", ag[1]);
			test[1] = "-e";
			test[2] = NULL;
			execve(test[0], test, env);
			return 0;
		}
        write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
        free(str);
    }
}

int main(int ac, char **ag, char **env)
{
	int fd;

	if (!(strcmp(ag[3],">")))
		exec_out_rdr_replace(fd, ag, env);
	else if (!(strcmp(ag[3],">>")))
		exec_out_rdr_append(fd, ag, env);
	else if (!(strcmp(ag[2],"<")))
		exec_in_rdr(fd, ag, env);
	else if (!(strcmp(ag[2],"<<")))
		exec_in_heredoc(fd, ag, env);
	else
	    return 1;
	// while (1);
}
