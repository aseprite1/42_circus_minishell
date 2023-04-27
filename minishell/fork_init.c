/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 09:03:14 by geonlee           #+#    #+#             */
/*   Updated: 2023/04/27 18:58:43 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct s_redir
{
    int dir; // "<","<<",">",">>" 구분용
    int is_end;
    char *arg; // "리다이렉션 뒤에 오는 인자"
}   t_redir;

typedef struct s_command
{
    char **cmd; // ex) { {/"bin/ls/"}, {"-la"}}
    char **env;
    int is_redir; // 리다이렉션 여부
    int is_builtin; // 빌트인 여여부
    int is_end;
    int idx; // 파이프 기준으로 나눴을때 몇번째 커멘드인지
    t_redir *in;
    t_redir *out;
}   t_command;

int pipe_cnt(t_command *command)
{
    int i;

    i = 0;
    if (!command)
        return (-1);
    while (command[i].is_end != 1)
        i++;
    return (i);
}

void wait_all_process(pid_t *pid_lst, int pipe_len)
{
    int i;
    int exit_code;

    i = 0;
    while (i < pipe_len)
    {
        waitpid(pid_lst[i], &exit_code, 0);
        i++;
    }
    free(pid_lst);
}

void builtin_exec(t_command command)
{
    return ;
}

void exec_in_heredoc(t_command command, int i)
{
	char	*str;
    int     fd;
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
		if (!strcmp(str, command.out[i].arg))
		{
			free(str);
			if (close(fd) == -1)
				return (1);
			fd = open("heredoc", O_RDONLY , 0644);
			dup2(fd, STDIN_FILENO);
			if (close(fd) == -1)
				return (1);
			//  execve(command.cmd[0], command.cmd, command.env);;
			return 0;
		}
        write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
        free(str);
    }
}

void    exec_in_rdr(t_command command, int i)
{
	int fd;
    
    fd = open(command.in[i].arg, O_RDONLY, 0644);
    dup2(fd, STDIN_FILENO);
    close(fd);
    // execve(command.cmd[0], command.cmd, command.env);
}

void    exec_out_rdr_replace(t_command command, int i)
{
    int fd;

    fd = open(command.out[i].arg, O_TRUNC | O_CREAT | O_WRONLY, 0644);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    // execve(command.cmd[0], command.cmd, command.env);
}

void    exec_out_rdr_append(t_command command, int i)
{
    int fd;

    fd = open(command.out[i].arg, O_APPEND | O_CREAT | O_WRONLY, 0644);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    // execve(command.cmd[0], command.cmd, command.env);
}

void exec_redir(t_command command)
{
    int i;
    t_redir *out;
    t_redir *in;

    i = 0;
    if (command.in)
    {
        while (1)
        {
            if (command.in[i].dir == 1)
                exec_in_rdr(command, i);
            else
                exec_in_heredoc(command, i);
            if (command.out[i].is_end == 1)
                break;
            i++;
        }
    }
    i = 0;
    if (command.out)
        while (1)
        {
            if (command.out[i].dir == 1)
                 exec_out_rdr_replace(command, i);
            else
                 exec_out_rdr_append(command, i);
            if (command.out[i].is_end == 1)
                break;
            i++;
        }
    execve(command.cmd[0], command.cmd, command.env);
}

void exec_cmd(t_command command)
{   
    if (command.is_redir == 1)
        exec_redir(command);
    else
    {
        if (command.is_builtin == 0)
            execve((command.cmd)[0], command.cmd, command.env);
        else
            builtin_exec(command);
    }
}s

void pipe_child_process(t_command *command, int *pipes, int in_tmp, int i)
{
    if (i > 0)
    {
        dup2(in_tmp, STDIN_FILENO);
        close(in_tmp);
    }
    if (command[i].is_end == 1)
    {
        close(pipes[0]);
        close(pipes[1]);  
        exec_cmd(command[i]);       
    }
    else
    {
        dup2(pipes[1], STDOUT_FILENO);
        close(pipes[0]);
        close(pipes[1]);
        exec_cmd(command[i]);
    }
}

pid_t *fork_process(t_command *command, int pipe_len)
{
    pid_t *pid;
    int pipes[2];
    int in_tmp;
    int i;

    i = 0;
    pid = (pid_t *)malloc(sizeof(pid_t) * (pipe_len + 1));
    if (!pid)
        exit(1);
    while (pipe_len >= i)
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

void fork_init(t_command *command)
{
    pid_t   *pid_lst;
    int     pipe_len;
    int     stdin_fd = 0;
    int     stdout_fd = 1;

    pipe_len = pipe_cnt(command);
    printf("pipe_len : %d\n", pipe_len);
    if (pipe_len == 0)
        exec_cmd(command[0]);
    pid_lst = fork_process(command, pipe_len);
    wait_all_process(pid_lst, pipe_len);
}


void test_init(t_command *command, char **ag)
{
    char **ls;
    char **cmd_grep;
    ls = (char **)malloc(sizeof(char *) * 3);
    ls[0]= strdup(ag[1]);
    ls[1]= strdup(ag[2]);
    ls[2]= NULL;
    command[0].cmd = ls;
    command[0].is_redir = 1;
    command[0].is_builtin = 0;
    command[0].idx = 0;
    command[0].is_end = 0;
    command[0].in = NULL;
    command[0].out = NULL;
    command[0].env = NULL;



    // cmd_grep = (char **)malloc(sizeof(char *) * 3);
    // cmd_grep[0] = strdup(ag[3]);
    // cmd_grep[1] = strdup(ag[4]);
    // cmd_grep[2] = NULL;
    // command[1].cmd = cmd_grep;
    // command[1].is_redir = 0;
    // command[1].is_builtin = 0;
    // command[1].idx = 1;
    // command[1].is_end = 1;
    // command[1].in = NULL;
    // command[1].out = NULL;    
    // command[1].env = NULL;   
}

int main(int ac, char **ag)
{
    t_command *command;

    printf("%d \n",ac);
    command = (t_command *)malloc(sizeof(t_command) * 4);
    test_init(command, ag);
    printf("1[0] : %s\n",(command[0].cmd)[0]);
    printf("1[1] : %s\n",(command[0].cmd)[1]);
    printf("wut : %d\n",(command[0].idx));
    // printf("wut : %d\n",(command[1].idx));
    // printf("2[0] : %s\n",(command[1].cmd)[0]);
    // printf("2[1] : %s\n",(command[1].cmd)[1]);
    fork_init(command);
}