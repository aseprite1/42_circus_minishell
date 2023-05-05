
int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}

void	error_exit(char *arg, int pipe_len)
{
	if (!pipe_len)
		printf("exit\n");
    if (arg)
    {
        printf("%s: exit: %s: numeric argument required\n", SHELL, arg); // 나가짐 
        exit(255);
    }
    else
    {
        // exit code 1 
        printf("%s: exit: too many arguments\n", SHELL);
    }
}

void	ft_exit(char **arg, int pipe_len)
{
	int	i;

	if (arg[1])
	{
		i = 0;
		if (arg[1] && (arg[1][0] == '-' || arg[1][0] == '+'))
			++i;
		while (arg[1][i] && ft_isdigit(arg[1][i]))
			++i;
		if (i < ft_strlen(arg[1]))
			error_exit(arg[1], pipe_len);
		if (arg[2])
			error_exit(NULL, pipe_len);
		else
            if (!pipe_len)
		        printf("exit\n");
            exit(ft_atoi(arg[1]));
	}
	else
	{
        if (!pipe_len)
		    printf("exit\n");
        exit(0);
    }
}