#include "minishell.h"

void	free_env_lst(t_env *env)
{
	t_env	*mem;

	if (env)
		mem = env->next;
	while (env)
	{
		free(env->name);
		free(env->value);
		free(env);
		env = mem;
		if (env)
			mem = env->next;
	}
}

char	*get_simplified_arg(char *s)
{
	int		i;
	int		first_occurence;
	char	*res;
	int		j;

	j = 0;
	first_occurence = 0;
	i = -1;
	res = xmalloc(ft_strlen(s) * sizeof(char));
	while (s[++i])
	{
		if (s[i] == '=')
			first_occurence = 1;
		if (first_occurence || s[i] != '+')
			res[j++] = s[i];
	}
	res[j] = 0;
	return (res);
}

void	append_env(t_env *env, char *s, t_shell *shell)
{
	char	*tmp;
	char	*tmp2;

	tmp = NULL;
	shell->tmp = get_simplified_arg(s);
	while (env)
	{
		if (!ft_strncmp(shell->tmp, env->name, ft_strlen(env->name)))
		{
			tmp2 = get_side(RIGHT, shell->tmp);
			tmp = ft_strjoin(env->value, tmp2);
			free(env->value);
			free(tmp2);
			if (!tmp)
				exit(1);
			env->value = tmp;
		}
		env = env->next;
	}
	free(shell->tmp);
}

int	change_value(t_env *env, char *new_value)
{
	free(env->value);
	env->value = new_value;
	return (new_value == 0);
}

char	*get_side(int side, char *s)
{
	int		i;
	char	*res;
	int		j;

	j = 0;
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	if (side == RIGHT)
	{
		res = xmalloc(sizeof(char) * (ft_strlen(s) - i + 1));
		while (s[i] && s[++i])
			res[j++] = s[i];
		res[j] = 0;
		return (res);
	}
	res = xmalloc(sizeof(char) * (i + 2));
	i = 0;
	res[i] = s[i];
	while (s[++i] && s[i] && s[i - 1] != '=')
		res[i] = s[i];
	res[i] = 0;
	return (res);
}
