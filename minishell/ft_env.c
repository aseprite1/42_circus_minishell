
#include <stdio.h>
#include <stdlib.h>

void	ft_env(char **arg, char **env, int pipe_len)
{
    int n;

    n = 0;
	if (arg[1])
	{
		printf("env: %s: No such file or directory\n", arg[1]);
		if (pipe_len)
			exit(127);
		//exitcode 127
		return ;
	}
	while (env[n])
	{
        printf("%s\n",env[n]); // env[n]에 = 있는지 검사하여 출력 필요할수도 ?
		n++;
	}
	if (pipe_len)
		exit(0);
	//exitcode 0
}

int main(int ac, char **ag, char **env)
{
    ft_env(ag,env,0);
}