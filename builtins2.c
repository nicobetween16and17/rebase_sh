#include "minishell.h"

int	valid_identifier(char *s)
{
	int	i;
	int	first_occurence;
	int	check;

	check = 0;
	first_occurence = 0;
	i = -1;
	while (s[++i])
	{
		if (!check && first_occurence)
			return (0);
		((s[i] == '=' && first_occurence++) || ((s[i] != '=' && check != '"' \
		&& check != '\'' && check != '?') && check++));
		if (!first_occurence && s[i] == '+' && s[i + 1] == '=')
			return (2);
		if (!first_occurence && !ft_isalnum(s[i]) && s[i] != '_' && \
		!(s[i] == '+' && s[i + 1] == '='))
			return (print_err(3, "minishell: export: '", s, \
			"': not a valid identifier\n") - 1);
	}
	if (!first_occurence)
		return (3);
	return (1);
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
 * displays the environment variable in the alphabetical order and displays
 * "declare -x" in front of each
 */
int	env_in_alphabetic_order(t_env *env)
{
	char	*last;
	t_env	*current;

	last = "\0";
	while (19)
	{
		current = get_next_smallest(env, last);
		if (current)
			last = current->name;
		else
			break ;
		ft_printf("declare -x %s%s\n", current->name, current->value);
	}
	return (0);
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