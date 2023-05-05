#include "minishell.h"

void	ft_pwd(int pipe_len)
{
	char	*pwd;

	pwd = get_env("PWD");
    if (pwd==NULL)
    {
        if (pipe_len)
			exit(1);
    }
	else
	{
		write(1, pwd, ft_strlen(pwd));
		write(1, "\n", 1);
		free(pwd);
		if (pipe_len)
			exit(0);
		//exitcode 0
	}
}