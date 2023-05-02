/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonlee <geonlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 12:10:40 by jaeyan            #+#    #+#             */
/*   Updated: 2023/05/02 19:51:07 by geonlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdio.h>
#include<stdlib.h>
// #include<readline.h>
// #include<history.h>

int		g_exit = 0;

typedef struct s_redir
{
	int		dir;
	int		dir_len;
	char	*heredoc_file;
	char	*arg;
}	t_redir;

typedef struct s_command
{
	char	**cmd;
	char	**env;
	int		is_redir;
	int 	builtin_num;
	int		idx;
	int		command_len;
	t_redir	*redir;
}	t_command;

typedef struct s_line
{
	int		err;
	int		pipe;
	char	**str;
	char	*tok;
}	t_line;

t_line	reading_line(char *line);
int		str_len(char *str);
int		count_str(char *line);
char	**del_space(char *line, int count);
char	*is_str(char *str);
int		skip_single(char *str);
int		skip_double(char *str);
int		check_empty(char *line);
int		is_operator(char c);
void	str_lcpy(char *str, char *line, int ii);
int		parse(char *str, char **envp);
int		mk_cmds(t_command *rt, int dir_num, int end, t_line pac);
int		mk_cmd(t_command *rt, int i, t_line pac);

void	cl(void)
{
	system("leaks a.out");
}

char			**cpy_env(char **env)
{
	char	**rt;
	int		i;

	i = 0;
	while (env[i])
		i++;
	if (!(rt = malloc(sizeof(char *) * (i + 1))))
		return (NULL);
	i = -1;
	while (env[++i])
		rt[i] = ft_strdup(env[i]);
	rt[i] = NULL;
	return (rt);
}

int	main(int ac, char **av, char **ev)
{
	char	**env;

	env = cpy_env(ev);
	if (!env)
		return (1);
	
	
	parse("<<  \"$USER\"  >> echo echo | cat |  \" \" | << '' ", env);
	return (0);
}

void	del_c(char *c)
{
	int	i;

	i = 0;
	while (c[i])
	{
		c[i] = c[i + 1];
		++i;
	}
}

int	del_single(char *str)
{
	int	i;

	i = 0;
	del_c(str);
	while (str[i] != 39)
	{
		if (!str[i])
			return (-1);
		++i;
	}
	del_c(str + i);
	return (i - 1);
}

int	del_double(char *str)
{
	int	i;

	i = 0;
	del_c(str);
	while (str[i] != 34)
	{
		if (!str[i])
			return (-1);
		++i;
	}
	del_c(str + i);
	return (i - 1);
}

void	del_quote(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == 34)
			i += del_double(str + i);
		else if (str[i] == 39)
			i += del_single(str + i);
		++i;
	}
}

int	env_rule_check(char c, int i)
{
	if (c <= 'Z' && c >= 'A')
		return (1);
	else if (c <= 'z' && c >= 'a')
		return (1);
	else if (c == '_')
		return (1);
	if (i && (c <= '9' && c >= '0'))
		return (1);
	return (0);
}

int	count_digits(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	while (!n)
	{
		++i;
		n /= 10;
	}
	return (n);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		digit;

	digit = count_digits(n);
	str = malloc(digit + 1);
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

char	*ch_env(char *str, int *i)
{
	int		j;
	char	*tmp;
	char	*env;

	j = 1;
	env = ft_itoa(g_exit);
	if (!env)
		return (str);
	j = str_len(env);
	tmp = malloc(str_len(str) + j + 1);
	if (!tmp)
	{
		free (env);
		return (str);
	}
	str_lcpy(tmp, str, *i);
	str_lcpy(tmp + *i, env, j);
	str_lcpy(tmp + *i + j, str + (*i) + 2, str_len(str + (*i) + j));
	free (str);
	free (env);
	str = tmp;
	(*i) = (*i) + j - 1;
	return (str);
}

char	*exch_env(char *str, char *env, int *i, int j)
{
	char	*tmp;

	tmp = malloc(str_len(str) - j + str_len(env) + 1);
	if (!tmp)
		return (str);
	str_lcpy(tmp, str, *i);
	str_lcpy(tmp + *i, env, str_len(env));
	str_lcpy(tmp + *i + str_len(env), str + (*i) + j, str_len(str + (*i) + j));
	free (str);
	str = tmp;
	(*i) = (*i) + str_len(env) - 1;
	return (str);
}

char	*ex_env(char *str, int *i, int j)
{
	char	*tmp;
	char	*env;

	if (str[*i] == '$' && str[(*i) + 1] == '?')
		return (ch_env(str, i));
	else if (str[*i] == '$' && env_rule_check(str[(*i) + 1], 0))
	{
		while (env_rule_check(str[(*i) + j], 1))
			++j;
		tmp = malloc(j);
		if (!tmp)
			return (str);
		str_lcpy(tmp, str + (*i) + 1, j - 1);
		env = getenv(tmp);
		free (tmp);
		if (!env || env[0] == 0)
		{
			while (j--)
				del_c(str + (*i));
			--(*i);
		}
		else
			return (exch_env(str, env, i, j));
	}
	return (str);
}

char	*del_double_chenv(char *str, int *i)
{
	del_c(str + (*i));
	while (str[*i] != 34)
	{
		if (!str[(*i)])
			return (str);
		if (str[(*i)] == '$')
			str = ex_env(str, i, 1);
		++(*i);
	}
	del_c(str + (*i));
	--(*i);
	return (str);
}

char	*del_quote_chenv(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == 39)
			i += del_single(str + i);
		else if (str[i] == 34)
			str = del_double_chenv(str, &i);
		else if (str[i] == '$')
		{
			if (str[i + 1] == '"' || str[i + 1] == '\'' )
			{
				del_c(str + i);
				--i;
			}
			else
				str = ex_env(str, &i, 1);
		}
		++i;
	}
	return (str);
}

int	export_arg_checker(char *str)
{
	int	i;

	i = 0;
	if (env_rule_check(str[i], 1))
		return (1);
	while (str[i] || str[i] != '=')
	{
		if (env_rule_check(str[i], 0))
			return (1);
		++i;
	}
	return (0);
}

int		ch_op(char *str)
{
	if (str[0] == '|' && !str[1])
		return ('1');
	if (str[0] == '<' && !str[1])
		return ('2');
	if (str[0] == '>' && !str[1])
		return ('4');
	if (str[0] == '<' && str[1] == '<' && !str[2])
		return ('3');
	if (str[0] == '>' && str[1] == '>' && !str[2])
		return ('5');
	return ('6');
}

char	*mk_tkn(char *sp[], int i)
{
	char	*rt;

	rt = malloc(i + 1);
	if (!rt)
		return (0);
	i = 0;
	while (sp[i])
	{
		if (is_operator(sp[i][0]))
			rt[i] = ch_op(sp[i]);
		else
			rt[i] = '7';
		++i;
	}
	rt[i] = 0;
	return (rt);
}

void	syn_error(char *c)
{
	printf("syntax error near unexpected token `%s\'\n", c);
}

int	syntex_check(char *c, int i, int *pipe, char **str)
{
	(*pipe) = 1;
	while (c[i])
	{
		if (c[i] == '1')
			++(*pipe);
		if (c[0] == '1' || c[i] == '6' || (c[i] == '1' && c[i + 1] == '1'))
		{
			syn_error(str[i]);
			return (1);
		}
		else if ((c[i] >= '1' && c[i] <= '5') && !c[i + 1])
		{
			syn_error("newline");
			return (1);
		}
		else if ((c[i] >= '2' && c[i] <= '5') && c[i + 1] != '7')
		{
			syn_error(str[i + 1]);
			return (1);
		}
		++i;
	}
	return (0);
}

t_line	errt(t_line *pac, int i)
{
	t_line	rt;
	int		j;

	rt.err = i;
	rt.pipe = 0;
	rt.str = NULL;
	rt.tok = NULL;
	j = 0;
	if (!i)
		return (rt);
	if (i >= 2)
	{
		while (pac->str[j])
			free (pac->str[j++]);
		free (pac->str);
	}
	if (i >= 3)
		free (pac->tok);
	return (rt);
}

t_line	reading_line(char *line)
{
	t_line	pac;
	int		i;

	pac.err = 0;
	if (check_empty(line))
		return (errt(&pac, 0));
	i = count_str(line);
	pac.str = del_space(line, i);
	if (!(pac.str))
		return (errt(&pac, 1));
	pac.tok = mk_tkn(pac.str, i);
	if (!(pac.tok))
		return (errt(&pac, 2));
	if (syntex_check(pac.tok, 0, &pac.pipe, pac.str))
		return (errt(&pac, 3));
	i = 0;
	while (pac.tok[i])
	{
		if (pac.tok[i] == '7')
			pac.str[i] = del_quote_chenv(pac.str[i]);
		else if (pac.tok[i] == '3' && pac.tok[i + 1] == '7')
			del_quote(pac.str[++i]);
		++i;
	}
	return (pac);
}

int	str_cmp(char *one, char *two)
{
	int	i;

	i = 0;
	while (two[i])
	{
		if (one[i] != two[i])
			return (1);
		++i;
	}
	if (one[i] != two[i])
		return (1);
	return (0);
}

int	builtin_check(char *str)
{
	if (!str_cmp(str, "echo"))
		return (1);
	if (!str_cmp(str, "cd"))
		return (2);
	if (!str_cmp(str, "pwd"))
		return (3);
	if (!str_cmp(str, "export"))
		return (4);
	if (!str_cmp(str, "unset"))
		return (5);
	if (!str_cmp(str, "env"))
		return (6);
	if (!str_cmp(str, "exit"))
		return (7);
	return (0);
}

void	unpac(t_line *pac)
{
	int	i;

	i = 0;
	while (pac->tok[i])
	{
		if (pac->tok[i] != '7')
			free (pac->str[i]);
		++i;
	}
	free (pac->tok);
	free (pac->str);
}

int	errctl(t_line *pac, t_command **rt, int i)
{
	if (!rt || (pac->err < 4 && pac->err))
	{
		g_exit = 258;
		if (!pac->err)
			errt(pac, 3);
		if (rt)
			free (rt);
		return (1);
	}
	if (pac->err == 200)
		free (rt[i]->cmd);
	--i;
	while (i >= 0)
	{
		free (rt[i]->cmd);
		free (rt[i]->redir);
		--i;
	}
	i = 0;
	while (pac->tok[i])
		free (pac->str[i++]);
	free (pac->tok);
	free (pac->str);	
	return (2);
}

int	parse(char *str, char **envp)
{
	t_line		pac;
	t_command	*rt;
	int			i;

	i = 0;
	pac = reading_line(str, envp);
	if (pac.err == 0 && pac.tok == NULL)
		return (0);
	rt = malloc(sizeof(t_command) * pac.pipe);
	if (!rt || pac.err)
		return (errctl(&pac, &rt, -1));
	while (i < pac.pipe)
	{
		rt[i].env = envp;
		rt[i].idx = i;
		rt[i].command_len = pac.pipe;
		pac.err = mk_cmd(&rt[i], i, pac);
		if (pac.err)
			return (errctl(&pac, &rt, i));
		++i;
	}
	unpac(&pac);
	return (0);
}

void	wr_cmd(char **str, int start, int end, t_line pac)
{
	int	i;

	i = 0;
	while (start < end)
	{
		if (pac.tok[start] == '7')
		{
			str[i] = pac.str[start];
			++i;
		}
		else
			++start;
		++start;
	}
	str[i] = NULL;
}

void	wr_dir(t_redir *dir, t_line pac, int *s, int e)
{
	while (*s < e)
	{
		if (pac.tok[*s] >= '2' && pac.tok[*s] <= '5')
		{
			dir->dir = ((int)pac.tok[*s]) - '2';
			dir->arg = pac.str[*s];
			break ;
		}
		++(*s);
	}
	++(*s);
}

int	mk_cmds(t_command *rt, int dir_num, int end, t_line pac)
{
	int	start;

	start = pac.pipe;
	rt->cmd = malloc(sizeof(char *) * ((end - start + 1) - dir_num * 2));
	if (!rt->cmd)
		return (100);
	wr_cmd(rt->cmd, start, end, pac);
	if (rt->cmd[0])
		rt->builtin_num = builtin_check(rt->cmd[0]);
	if (rt->is_redir)
	{
		rt->redir = malloc(sizeof(t_redir) * rt->is_redir);
		if (!rt->redir)
			return (200);
		start = 0;
		while (dir_num--)
		{
			wr_dir(&(rt->redir[start]), pac, &start, end);
			rt->redir[start].dir_len = rt->is_redir;
			++start;
		}
	}
	return (0);
}

int	mk_cmd(t_command *rt, int i, t_line pac)
{
	int			j;
	int			count;

	j = 0;
	count = 0;
	while (i--)
	{
		while (pac.tok[j] != '1')
			++j;
		++j;
	}
	pac.pipe = j;
	while (pac.tok[j] && pac.tok[j] != '1')
	{
		if (pac.tok[j] >= '2' && pac.tok[j] <= '5')
			++count;
		++j;
	}
	rt->is_redir = count;
	rt->builtin_num = 0;
	i = mk_cmds(rt, count, j, pac);
	return (i);
}

int	str_len(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		++i;
	return (i);
}

int count_str(char *line)
{
	int		count;
	int		i;
	char	*s;

	i = 0;
	count = 0;
	while (line[i])
	{
		while (line[i] && line[i] == ' ')
			++i;
		s = is_str(line + i);
		if (!s)
			break ;
		++count;
		i += str_len(s);
		free (s);
	}
	return (count);
}

char **del_space(char *line, int count)
{
	int		i;
	int		n;
	char	**sp;

	i = 0;
	n = 0;
	sp = malloc(sizeof(char *) * (count + 1));
	if (!sp)
		return (0);
	while (line[i] && n < count)
	{
		while (line[i] && (line[i] == ' ' || line[i] == 9))
			++i;
		sp[n] = is_str(line + i);
		if (!sp[n])
		{
			while (n)
				free(sp[--n]);
			free(sp);
			return (0);
		}
		i += str_len(sp[n++]);
	}
	sp[n] = 0;
	return (sp);
}

void	str_lcpy(char *str, char *line, int ii)
{
	int		i;

	i = 0;
	while (i < ii)
	{
		str[i] = line[i];
		++i;
	}
	str[i] = 0;
}

int	skip_op(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == c)
		++i;
	return (i);
}

int	is_operator(char c)
{
	if (c == '|')
		return (1);
	else if (c == '<')
		return (1);
	else if (c == '>')
		return (1);
	return (0);
}

char	*is_str(char *str)
{
	int		i;
	char	*rt;

	i = 0;
	if (str[i] == 0)
		return (0);
	while (str[i] && str[i] != ' ' && !is_operator(str[i]))
	{
		if (str[i] == 34)
			i += skip_double(str + i);
		else if (str[i] == 39)
			i += skip_single(str + i);
		++i;
	}
	if (i == 0)
		i += skip_op(str, str[0]);
	rt = malloc(i + 1);
	if (!rt)
		return (0);
	str_lcpy(rt, str, i);
	return (rt);
}

int	skip_single(char *str)
{
	int		i;

	i = 1;
	while (str[i] != 39)
	{
		if (!str[i])
			return (0);
		++i;
	}
	return (i);
}

int	skip_double(char *str)
{
	int		i;

	i = 1;
	while (str[i] != 34)
	{
		if (!str[i])
			return (0);
		++i;
	}
	return (i);
}

int	check_empty(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != 9)
			return (0);
		++i;
	}
	return (1);
}
