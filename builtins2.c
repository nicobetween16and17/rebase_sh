#include "minishell.h"

int	built_in(char **cmd, t_shell *sh)
{
	int	is_built_in;

	is_built_in = 0;
	if (!ft_strncmp(cmd[0], "echo", 5) && ++is_built_in)
		g_signal.ret = ft_echo(cmd, sh);
	else if (!ft_strncmp(cmd[0], "cd", 3) && ++is_built_in)
		g_signal.ret = ft_cd(cmd, sh);
	else if (!ft_strncmp(cmd[0], "pwd", 4) && ++is_built_in)
		g_signal.ret = ft_pwd(cmd, sh);
	else if (!ft_strncmp(cmd[0], "export", 7) && ++is_built_in)
		g_signal.ret = ft_export(cmd, sh);
	else if (!ft_strncmp(cmd[0], "unset", 6) && ++is_built_in)
		g_signal.ret = ft_unset(cmd, sh);
	else if (!ft_strncmp(cmd[0], "env", 4) && ++is_built_in)
		g_signal.ret = ft_env(cmd, sh);
	else if (!ft_strncmp(cmd[0], "exit", 5) && ++is_built_in)
		g_signal.ret = ft_exit(cmd, sh);
	return (is_built_in);
}

t_env	*get_next_smallest(t_env *env, char *last)
{
	t_env	*save;

	save = NULL;
	while (env)
	{
		if (ft_strncmp(env->name, last, ft_strlen(env->name)) > 0 && \
		(!save || ft_strncmp(env->name, save->name, ft_strlen(env->name)) < 0))
			save = env;
		env = env->next;
	}
	return (save);
}

/*
 * check if the parameter is a valid and existing name of environment
 * and erase the line if it is
 */
int	ft_unset(char **params, t_shell *shell)
{
	int	i;

	i = 0;
	while (params[++i])
	{
		if (remove_env(shell, params[i]))
		{
			ft_putstr_fd("invalid identifier\n", 2);
			return (1);
		}
	}
	return (0);
}

/*
 * if no parameters, shows all the environment variables
 * if parameters, for each parameters, call change_var
 */
int	ft_export(char **params, t_shell *shell)
{
	int	i;
	int	check;

	if (!params[1])
	{
		env_in_alphabetic_order(shell->env);
		return (0);
	}
	i = 0;
	while (params[++i])
	{
		check = valid_identifier(params[1]);
		if (!check)
			return (1);
		if (check == 1 && !change_env(shell->env, params[i]))
			add_env(&shell->env, \
			new_env(get_side(LEFT, params[i]), get_side(RIGHT, params[i])));
		if (check == 2)
			append_env(shell->env, params[i], shell);
		if (check == 3)
			add_env_plus_equal(shell, params[i]);
	}
	return (0);
}

int	ft_env(char **params, t_shell *shell)
{
	t_env	*env;

	env = shell->env;
	if (params[1])
	{
		ft_putstr_fd("env: ", 1);
		ft_putstr_fd(params[1], 1);
		ft_putstr_fd(": No such file or directory\n", 1);
		return (127);
	}
	else
	{
		while (env)
		{
			ft_printf("%s%s\n", env->name, env->value);
			env = env->next;
		}
	}
	return (0);
}
