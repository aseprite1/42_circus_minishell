

#ifndef TEST_H
# define TEST_H

#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#include<signal.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

typedef struct s_redir
{
	int dir; // "<","<<",">",">>" 구분용
	int dir_len;
	char *heredoc_file;
	char *arg; // "리다이렉션 뒤에 오는 인자"
}   t_redir;

typedef struct s_command
{
	char **cmd; // ex) { {/"bin/ls/"}, {"-la"}}
	char **env; //요기요 
	int is_redir; // 리다이렉션 여부
	int builtin_num; // 빌트인 여여부
	int command_len;
	int idx; // 파이프 기준으로 나눴을때 몇번째 커멘드인지
	t_redir *redir;
}   t_command;

typedef struct s_line
{
	int		err;
	int		pipe;
	char	**str;
	char	*tok;
}	t_line;


int		ft_strlen(const char *s);
char	*ft_strjoin(char *s1, char *s2);
void	wait_all_process(pid_t *pid_lst, int command_len);
char	*make_heredoc_temp(void);
void	write_in_heredoc(t_redir *redir);
void	exec_rdr_list(t_command *command);
void    exec_in_rdr(t_command command, int i);
void	builtin_exec(t_command command);
void	exec_in_heredoc(t_command command, int i);
void	exec_out_rdr_replace(t_command command, int i);
void	exec_out_rdr_append(t_command command, int i);
void 	exec_rdr(t_command command);
void	exec_cmd(t_command command, int stdout_tmp);
void	pipe_child_process(t_command *command, int *pipes, int in_tmp, int i);
pid_t	*fork_process(t_command *command);
void	exec_init(t_command *command);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	**ft_split(char const *s, char c);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strdup(const char *s1);
char	**cpy_env(char **env);
void	sig_handle(int signum);
char	**ready_to_go(int ac, char **av, char **ev);
char	*ft_itoa(int n);
int	count_digits(int n);
void	del_c(char *c);
int	del_single(char *str);
int	del_double(char *str);
void	del_quote(char *str);
int	env_rule_check(char c, int i);
int	check_empty(char *line);
int	skip_double(char *str);
int	skip_single(char *str);
char	*is_str(char *str);
int	is_operator(char c);
int	skip_op(char *str, char c);
void	str_lcpy(char *str, char *line, int ii);
char	**del_space(char *line, int count);
int		 count_str(char *line);
int		str_len(char *str);
int		mk_cmd(t_command *rt, int i, t_line pac);
int		mk_cmds(t_command *rt, int dir_num, int end, t_line pac);
void	wr_dir(t_redir *dir, t_line pac, int *s, int e);
void	wr_cmd(char **str, int start, int end, t_line pac);
char	**parse(char *str, char **envp);
void		errctl(t_line *pac, t_command **rt, int i);
void	unpac(t_line *pac);
int		builtin_check(char *str, t_command *rt, char **env);
int		path_finder(char *str, char **env, int i, t_command *rt);
int		found_path(char **sp, int i, t_command *rt, char *cmd);
int		str_cmp(char *one, char *two);
t_line	reading_line(char *line, char **envp);
t_line	errt(t_line *pac, int i);
int		syntex_check(char *c, int i, int *pipe, char **str);
void	syn_error(char *c);
char	*mk_tkn(char *sp[], int i);
int		ch_op(char *str);
int		export_arg_checker(char *str);
char	*del_quote_chenv(char *str, char **envp);
char	*del_double_chenv(char *str, int *i, char **envp);
char	*ex_env(char *str, int *i, int j, char **envp);
char	*get_env(char *find, char **env);
char	*env_cmp(char *one, char *two);
char	*exch_env(char *str, char *env, int *i, int j);
char	*ch_env(char *str, int *i);
void	ft_echo(char **arg, int pipe_len);
int	is_echo_flag(char *str);
void	ft_cd(char **arg, char **env, int pipe_len);
void	ft_env(char **arg, char **env, int pipe_len);
void	ft_pwd(char **env, int pipe_len);
void	ft_exit(char **arg, int pipe_len);
#endif
