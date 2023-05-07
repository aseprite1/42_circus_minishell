/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 09:03:14 by geonlee           #+#    #+#             */
/*   Updated: 2023/05/06 09:23:48 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void exit_with_code(int i)
{
	g_exit = i;
	exit(i);
}

int	ft_strlen(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strjoin(char *s1, char *s2)
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

void wait_all_process(pid_t *pid_lst, int command_len)
{
	int i;
	int exit_code;

	i = 0;
	while (i < command_len)
	{
		waitpid(pid_lst[i], &exit_code, 0);
		i++;
	}
	free(pid_lst);
}

char *make_heredoc_temp(void)
{
	char	*file_idx;
	char    *name;
	int     i;

	i = 0;
	while (1)
	{
		file_idx = ft_itoa(i);
		if (file_idx == NULL)
			exit_with_code(1);
		name = ft_strjoin("tmp_", file_idx);
		if (name == NULL)
			exit_with_code(1);
		if (access(name, F_OK) == -1)
		{
			free(file_idx);
			return (name);
		}
		free(name);
		free(file_idx);
		i++;
	}
}

void write_in_heredoc(t_redir *redir)
{
	int		fd;
	char	*str;
	char	*name;

	fd = open(redir->heredoc_file, O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
		exit_with_code(1);
	while (1)
	{
		str = readline("> ");
		if (!str)
		{
			close(fd);
			exit_with_code(0);
		}
		if (!str_cmp(str, redir->arg))
		{
			free(str);
			close(fd);
			exit_with_code(0);
		}
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		free(str);
	}
}

void exec_rdr_list(t_command *command)
{
	pid_t   pid;
	int     exit_code;
	int     i;
	int     j;
	
	i = 0;
	while (i < command[0].command_len)
	{
		j = 0;
		while (command[i].is_redir == 1 && j < command[i].redir[0].dir_len)
		{
			if (command[i].redir[j].dir == 1)
			{		
				pid = fork();
				command[i].redir[j].heredoc_file = make_heredoc_temp();
				if (pid > 0)
					waitpid(pid, &exit_code, 0);
				else if (pid == 0)
					write_in_heredoc(&(command[i].redir[j]));
			}
			j++;
		}
		i++;
	}
}

void builtin_exec(t_command command)
{
	if (command.builtin_num == 1)
		ft_echo(command.cmd, command.command_len - 1);
	else if (command.builtin_num == 2)
		ft_cd(command.cmd, command.env, command.command_len - 1);
	else if (command.builtin_num == 3)
		ft_pwd(command.env, command.command_len - 1);
	// else if (command.builtin_num == 4)
	// 	ft_export();
	// else if (command.builtin_num == 5)
	// 	ft_unset();
	else if (command.builtin_num == 6)
		ft_env(command.cmd, command.env, command.command_len - 1);
	else if (command.builtin_num == 7)
		ft_exit(command.cmd, command.command_len - 1);
	return ;
}

void    exec_in_rdr(t_command command, int i) 
{
	int fd;

	if (access(command.redir[i].arg, F_OK) == -1)
	{
		printf("minishell : %s: No such file or directory",command.redir[i].arg);
		exit_with_code(1);
	}
	if (access(command.redir[i].arg ,R_OK) == -1)
	{
		printf("minishell %s : Permission denied", command.redir[i].arg);
		exit_with_code(1); //errno 13  //
	}
	fd = open(command.redir[i].arg, O_RDONLY, 0644);
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	exec_in_heredoc(t_command command, int i)
{
	int fd;

	fd = open(command.redir[i].heredoc_file, O_RDONLY, 0644);
	dup2(fd, STDIN_FILENO);
	unlink(command.redir[i].heredoc_file);
	close(fd);
}

void    exec_out_rdr_replace(t_command command, int i)
{
	int fd;
	int access_code;
	access_code = access(command.redir[i].arg, F_OK);
	if (access_code != -1)
	{
		if (access(command.redir[i].arg ,R_OK) == -1)
		{
			printf("minishell %s : Permission denied", command.redir[i].arg);
			exit_with_code(1);
		}
	}
	fd = open(command.redir[i].arg, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (command.builtin_num != -1)
		dup2(fd, STDOUT_FILENO);
	close(fd);
}

void    exec_out_rdr_append(t_command command, int i)
{
	int fd;
	int access_code;

	access_code = access(command.redir[i].arg, F_OK);
	if (access_code != -1)
	{
		if (access(command.redir[i].arg ,R_OK) == -1)
		{
			printf("minishell %s : Permission denied", command.redir[i].arg);
			exit_with_code(1);
		}
	}
	fd = open(command.redir[i].arg, O_APPEND | O_CREAT | O_WRONLY, 0644);
	if (command.builtin_num != -1)
		dup2(fd, STDOUT_FILENO);
	close(fd);
}

void exec_rdr(t_command command)
{
	int i;
	
	i = 0;
	while (i < command.redir[0].dir_len)
	{
		if (command.redir[i].dir == 0)
			exec_in_rdr(command, i);
		else if (command.redir[i].dir == 1)
			exec_in_heredoc(command, i);
		else if (command.redir[i].dir == 2)
			exec_out_rdr_replace(command, i);
		else if (command.redir[i].dir == 3)
			exec_out_rdr_append(command ,i);
		i++;
	}
}


void exec_with_fork(t_command command)
{
	pid_t	pid;
	int		exit_code;
	
	pid = fork();
	if (pid > 0)
		waitpid(pid, &exit_code, 0);
	else if (pid == 0)
	{
		if (command.builtin_num == -1)
		{
			printf("minishell: %s: command not found\n",command.cmd[0]);
			exit(127);
		}
		if (command.is_redir == 1)
			exec_rdr(command);
		execve((command.cmd)[0], command.cmd, command.env);
	}
}


void exec_cmd(t_command command, int stdout_tmp)
{

	if (command.command_len == 1 && command.builtin_num < 1) // cmd not found, execve
	{
		exec_with_fork(command);
		return ;
	}
	if (command.is_redir == 1)
		exec_rdr(command);
	if (command.command_len == 1 && command.builtin_num > 0) //built_in 1 2 3 4 5 6 7
		builtin_exec(command);
	else if (command.command_len > 1  && command.builtin_num == -1)
	{	
		dup2(stdout_tmp, STDOUT_FILENO);
		close(stdout_tmp);
		printf("minishell: %s: command not found\n",command.cmd[0]);		
		//unlinking all heredocs here && exit code 127
		exit_with_code(127);
	}
	else if (command.command_len > 1  && command.builtin_num == 0)
		execve((command.cmd)[0], command.cmd, command.env);
	else if (command.command_len > 1  && command.builtin_num > 0)
		builtin_exec(command);
}

void pipe_child_process(t_command *command, int *pipes, int in_tmp, int i)
{
	int stdout_tmp;

	stdout_tmp = dup(STDOUT_FILENO);
	if (i > 0)
	{
		dup2(in_tmp, STDIN_FILENO);
		close(in_tmp);
	}
	if (i != (command[i].command_len - 1))
		dup2(pipes[1], STDOUT_FILENO);
	close(pipes[0]);
	close(pipes[1]);
	exec_cmd(command[i], stdout_tmp);
}


pid_t *fork_process(t_command *command)
{
	pid_t   *pid;
	int     pipes[2];
	int     in_tmp;
	int     i;

	i = 0;
	pid = (pid_t *)malloc(sizeof(pid_t) * command[0].command_len);
	if (!pid)
		exit_with_code(1);
	while (i < command[0].command_len)
	{   
		pipe(pipes);
		pid[i] = fork();
		if (pid[i] > 0)
		{
			if (i > 0)
				close(in_tmp);
			in_tmp = pipes[0];
			close(pipes[1]);
		}
		else if (pid[i] == 0)
			pipe_child_process(command, pipes, in_tmp, i);
		else
			exit_with_code(1);
		i++;
	}
	return (pid);
}

void exec_init(t_command *command)
{
	pid_t   *pid_lst;
	int		stdout_tmp;
	int 	stdin_tmp;

	// printf("command : %s\n",command[0].cmd[0]);
	// printf("flag : %s\n",command[0].cmd[1]);
	// printf("env[0] :  %s\n",command[0].env[0]);
	// printf("redir : %d\n",command[0].is_redir);
	// printf("builtin_num : %d\n",command[0].builtin_num);
	// printf("commandlen : %d\n",command[0].command_len);
	// printf("idx : %d\n",command[0].idx);
	// if (command[0].redir != NULL)
	// 	printf("dir = %d\n",command[0].redir[0].dir);
	// 	printf("dirlen = %d\n",command[0].redir[0].dir_len);
	// 	printf("filename = %s\n",command[0].redir[0].heredoc_file);
	// 	printf("arg = %s\n",command[0].redir[0].arg);

	exec_rdr_list(command);
	if (command[0].command_len == 1)
	{	
		stdin_tmp = dup(STDIN_FILENO);
		stdout_tmp = dup(STDOUT_FILENO);
		exec_cmd(command[0], stdout_tmp);
		dup2(stdout_tmp, STDOUT_FILENO);
		dup2(stdin_tmp, STDIN_FILENO);
		close(stdin_tmp);
		close(stdout_tmp);
	}
	else
	{
		pid_lst = fork_process(command);
		wait_all_process(pid_lst, command[0].command_len);
	}
}
