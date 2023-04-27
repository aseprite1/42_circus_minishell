/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 09:03:14 by geonlee           #+#    #+#             */
/*   Updated: 2023/04/25 20:30:36 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

typedef struct s_redir
{
    int dir; // "<","<<",">",">>" 구분용
    char *arg; // "리다이렉션 뒤에 오는 인자"
}   t_redir;

typedef struct s_command
{
    char **cmd; // ex) { {/"bin/ls/"}, {"-la"}}
    int is_redir; // 리다이렉션 여부
    int is_builtin; // 빌트인 여여부
    int idx; // 파이프 기준으로 나눴을때 몇번째 커멘드인지
    t_redir *in;
    t_redir *out;
}   t_command;

int pipe_cnt(t_command **command)
{
    int n;

    n = 0;
    if (!command)
        return (-1);
    while (command[n]!='\0')
        n++;
    n--;
    return (n);
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

void builtin_exec();

void exec_redir(t_command **command, int i)
{
    if 
}

void exec_cmd(t_command **command, int i)
{
    if (command[i]->is_redir == 1)
    {

    }
    else
    {
        if (command[i]->is_builtin == 0)
            execve((command[i]->cmd)[0],command[i]->cmd, NULL);
        else
            builtin_exec();
    }
}


void pipe_child_process(t_command **command, int *pipes, int in_tmp, int i)
{
    if (i > 0)
    {
        dup2(in_tmp, STDIN_FILENO);
        close(in_tmp);
    }
    dup2(pipes[1], STDOUT_FILENO);
    close(pipes[0]);
    close(pipes[1]);     
    exec_cmd(command, i);
}

pid_t *fork_process(int stdin_fd, t_command **command, int pipe_len)
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
        else if (pid == 0)
            pipe_child_process(command, pipes, in_tmp, i);
        else
            exit(1);
        i++;
    }
    return (pid);
}

void fork_init(t_command **command)
{
    pid_t   *pid_lst;
    int     pipe_len;
    int     stdin_fd;
    int     stdout_fd;

    pipe_len = pipe_cnt(command);
    stdin_fd = dup(STDIN_FILENO);
    stdout_fd = dup(STDOUT_FILENO);
    if (pipe_len == 0)
        execute_one_command(command);
    pid_lst = fork_process(stdin_fd, command, pipe_len);
    wait_all_process(pid_lst, pipe_len);
    dup2(stdin_fd, STDIN_FILENO);
    dup2(stdout_fd, STDOUT_FILENO);
    close(stdin_fd);
    close(stdout_fd);
}


void test_init(t_command *cmd)
{
    // char tmp_cmd[2][5] = {{ 'e', 'c', 'h', 'o' ,'\0' }, { 't', 'e', 's', 't','\0' }};
    // cmd->cmd = tmp_cmd;
    // rdr_init 
}

int main()
{
    t_command *command;
    test_init(&command);
    fork_init(&command);
}