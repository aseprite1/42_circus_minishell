/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 09:03:14 by geonlee           #+#    #+#             */
/*   Updated: 2023/04/30 02:10:40 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_redir
{
	int dir; // "<","<<",">",">>" 구분용
	int dir_len;
	int idx;
	char *heredoc_file;
	char *arg; // "리다이렉션 뒤에 오는 인자"
}   t_redir;

typedef struct s_command
{
	char **cmd; // ex) { {/"bin/ls/"}, {"-la"}}
	char **env;
	int is_redir; // 리다이렉션 여부
	int is_builtin; // 빌트인 여여부
	int command_len;
	int idx; // 파이프 기준으로 나눴을때 몇번째 커멘드인지
	t_redir *redir;
}   t_command;

int	ft_strlen(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

int count_digits(int num)
{
	int count;
	
	count = 0;
	if (num == 0) 
		return 1;
	while (num != 0) 
	{
		count++;
		num /= 10;
	}
	return (count) ;
}

char *ft_itoa(int n)
{
	char    *str;
	int     digit;

	digit = count_digits(n);
	str = (char *)malloc(digit + 1);
	if (!str)
		return (NULL);
	str[digit] = 0;
	while (digit-- > 0)
	{
		str[digit] = '0' + (n % 10);
		n /= 10;
	}
	return (str);
}

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

void wait_all_process(pid_t *pid_lst, int command_len)
{
	int i;
	int exit_code;

	i = 0;
	while (i < command_len)
	{
		// printf("i'm waiting pid %d : %d... \n",i,pid_lst[i]);
		waitpid(pid_lst[i], &exit_code, 0);
		// printf("done pid %d : %d... \n",i,pid_lst[i]);
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
			exit(1);
		name = ft_strjoin("tmp_", file_idx);
		if (name == NULL)
			exit(1);
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
		exit(1);
	while (1)
	{
		str = readline("> ");
		if (!str)
		{
			exit(0);
			close(fd);
		}
		if (!strcmp(str, redir->arg))
		{
			free(str);
			close(fd);
			exit(0);
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
	return ;
}

void    exec_in_rdr(t_redir redir)
{
	int fd;

	//있고 (없으면 없을떄 exitcode), 권한도 있는지(권한이없으면 없을 시에 exitcode)	
	fd = open(redir.arg, O_RDONLY, 0644);
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	exec_in_heredoc(t_redir redir)
{
	int fd;

	fd = open(redir.heredoc_file, O_RDONLY, 0644);
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void    exec_out_rdr_replace(t_redir redir)
{
	int fd;
	
	//권한도 있는지(권한이없으면 없을 시에 exitcode)	
	fd = open(redir.arg, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void    exec_out_rdr_append(t_redir redir)
{
	int fd;

	// 권한도 있는지(권한이없으면 없을 시에 exitcode)	
	fd = open(redir.arg, O_APPEND | O_CREAT | O_WRONLY, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void exec_rdr(t_redir *redir)
{
	int i;
	
	i = 0;
	while (i < redir[0].dir_len)
	{
		if (redir[i].dir == 0)
			exec_in_rdr(redir[i]);
		else if (redir[i].dir == 1)
			exec_in_heredoc(redir[i]);
		else if (redir[i].dir == 2)
			exec_out_rdr_replace(redir[i]);
		else if (redir[i].dir == 3)
			exec_out_rdr_append(redir[i]);
		i++;
	}
}

void exec_cmd(t_command command)
{
	if (command.is_redir == 1)
		exec_rdr(command.redir);
	if (command.is_builtin == 0)
		execve((command.cmd)[0], command.cmd, command.env);
	else
		builtin_exec(command);
}

void pipe_child_process(t_command *command, int *pipes, int in_tmp, int i)
{
	if (i > 0)
	{
		dup2(in_tmp, STDIN_FILENO);
		close(in_tmp);
	}
	if (i != (command[i].command_len - 1))
		dup2(pipes[1], STDOUT_FILENO);
	close(pipes[0]);
	close(pipes[1]);
	exec_cmd(command[i]);
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
		exit(1);
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
			exit(1);
		i++;
	}
	return (pid);
}

void exec_init(t_command *command)
{
	pid_t   *pid_lst;

	exec_rdr_list(command);
	if (/*커맨드 개수 1개 && builtin && (builit이 환경변수 바꿔주는 명령어일때 || exit))*/0)
	{
		//fork 안하고 처리. exec함수 아예 빠져나가서 새로운 프롬프트 띄우거나 exit이면 모두 free하고 종료되도록
	}
	else if (command[0].command_len == 1)
	{
		//fork 함 && wait해서 exit value 받아서 전역변수에 저장
		exec_cmd(command[0]);
	}
	else
	{
		pid_lst = fork_process(command);
		wait_all_process(pid_lst, command[0].command_len);
	}
}


void test_init(t_command *command, char **ag)
{
	char **ls;
	char **cmd_grep;
	char **cmd_gre;
	ls = (char **)malloc(sizeof(char *) * 3);
	ls[0]= strdup(ag[1]);
	ls[1]= strdup(ag[2]);
	ls[2]= NULL;
	command[0].cmd = ls;
	command[0].is_redir = 1; 
	command[0].is_builtin = 0;
	command[0].idx = 0;
	command[0].command_len = 2;
	command[0].env = NULL;
	t_redir *out_tmp = (t_redir *)malloc(sizeof(t_redir) * 2);
	out_tmp[0].dir=1;
	out_tmp[0].dir_len=2;
	out_tmp[0].heredoc_file=NULL;
	out_tmp[0].arg=strdup(ag[3]);
	out_tmp[1].dir=1;
	out_tmp[1].dir_len=2;
	out_tmp[1].heredoc_file=NULL;
	out_tmp[1].arg=strdup(ag[4]);	
	command[0].redir = out_tmp;
	//  command[0].redir = NULL;

	cmd_grep = (char **)malloc(sizeof(char *) * 3);
	cmd_grep[0] = strdup(ag[5]);
	cmd_grep[1] = strdup(ag[6]);
	cmd_grep[2] = NULL;
	t_redir *in_tmp2 = (t_redir *)malloc(sizeof(t_redir) * 1);
	// in_tmp2[0].dir=2;
	// in_tmp2[0].arg=strdup(ag[5]);
	command[1].cmd = cmd_grep;
	command[1].is_redir = 0;
	command[1].is_builtin = 0;
	command[1].idx = 1; 
	command[1].command_len = 2;
	command[1].env = NULL; 
	command[1].redir = NULL;
	// cmd_gre = (char **)malloc(sizeof(char *) * 3);
	// cmd_gre[0] = strdup(ag[5]);
	// cmd_gre[1] = strdup(ag[6]);
	// cmd_gre[2] = NULL;
	// command[2].cmd = cmd_gre;
	// command[2].is_redir = 0;
	// command[2].is_builtin = 0;
	// command[2].idx = 2;
	// command[2].is_end = 1;
	// command[2].in = NULL;
	// command[2].out = NULL;    
	// command[2].env = NULL;  
}

int main(int ac, char **ag)
{
	t_command *command;

	// printf("%d \n",ac);
	command = (t_command *)malloc(sizeof(t_command) * 4);
	test_init(command, ag);
	// printf("1[0] : %s\n",(command[0].cmd)[0]);
	// printf("1[1] : %s\n",(command[0].cmd)[1]);
	// printf("2[0] : %s\n",(command[1].cmd)[0]);
	// printf("2[1] : %s\n",(command[1].cmd)[1]);
	exec_init(command);
}