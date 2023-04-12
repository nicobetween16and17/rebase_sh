#include "minishell.h"

/*
 * change the current directory to the one in parameters
 */
int	ft_cd(char **params, t_shell *shell)
{
	shell->tmp = params[1];
	if (!shell->tmp)
	{
		shell->tmp = get_env_line(shell->env, "HOME=");
		if (shell->tmp && ft_strlen(shell->tmp) > 5)
			shell->tmp += 5;
		shell->dir = chdir(shell->tmp);
	}
	else
		shell->dir = chdir(shell->tmp);
	if (shell->dir)
	{
		shell->dir = open(shell->tmp, O_RDONLY);
		if (shell->dir > 0)
		{
			ft_putstr_fd("cd: not a directory: ", 2);
			ft_putstr_fd(shell->tmp, 2);
			ft_putstr_fd("\n", 2);
			return (close(shell->dir) + 1);
		}
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(shell->tmp, 2);
		ft_putstr_fd(": no such file or directory\n", 2);
	}
	return (ABS(shell->dir));
}

/*
 * gives the current directory's absolute path
 */
int	ft_pwd(char **params, t_shell *shell)
{
	char	*buf;

	(void)params;
	(void)shell;
	buf = getcwd(NULL, 0);
	ft_putstr_fd(buf, 1);
	write(1, "\n", 1);
	free(buf);
	return (0);
}

/*
 * return 0 except if the second one start
 * with '-' and only contains n
 */
int	only_n(char *s)
{
	int	i;

	i = -1;
	while (s[++i])
	{
		if ((i == 0 && s[i] != '-') || (i > 0 && s[i] != 'n'))
			return (0);
	}
	return (1);
}

int	ft_echo(char **args, t_shell *shell)
{
	int		i;
	int		n_option;

	(void)shell;
	i = 1;
	n_option = 0;
	if (array_size(args) > 1)
	{
		while (args[i] && only_n(args[i]) && ++n_option)
			i++;
		while (args[i])
		{
			ft_putstr_fd(args[i], 1);
			if (args[i + 1] && args[i][0] != '\0')
				write(1, " ", 1);
			i++;
		}
	}
	if (!n_option)
		write(1, "\n", 1);
	return (0);
}

/*
 * exit the program if 1 or less numeric parameters, else
 * send an error msg
 */
int	ft_exit(char **params, t_shell *shell)
{
	shell->exit = 1;
	ft_putstr_fd("exit\n", 2);
	if (params[1] && ft_strisnum(params[1]) == 0)
	{
		g_signal.ret = 255;
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(params[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
	}
	else if (params[1] && params[2])
	{
		g_signal.ret = 1;
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		shell->exit = 0;
	}
	else if (params[1])
		g_signal.ret = ft_atoi(params[1]);
	else
		g_signal.ret = 0;
	return (g_signal.ret);
}
