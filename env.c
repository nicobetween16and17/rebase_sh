#include "minishell.h"

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
t_env	*new_env(char *name, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (0);
	new->name = name;
	new->value = value;
	new->next = NULL;
	return (new);
}

t_env	*last_env(t_env *lst)
{
	if (!lst)
		return (lst);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

int	add_env(t_env **lst, t_env *new)
{
	t_env	*tmp;

	if (*lst == NULL)
		*lst = new;
	else
	{
		tmp = last_env(*(lst));
		tmp->next = new;
	}
	return (1);
}
t_env	*get_env_side(t_env *env_lst, char *s, int side)
{
	if (side == RIGHT)
	{
		while (env_lst && ft_strncmp(env_lst->name, s, ft_strlen(s)))
			env_lst = env_lst->next;
		if (env_lst)
			env_lst = env_lst->next;
	}
	else if (side == LEFT)
	{
		while (env_lst && env_lst->next && \
		ft_strncmp(env_lst->next->name, s, ft_strlen(s)))
			env_lst = env_lst->next;
	}
	else
	{
		while (env_lst && ft_strncmp(env_lst->name, s, ft_strlen(s)))
			env_lst = env_lst->next;
	}
	return (env_lst);
}

int	remove_env(t_shell *sh, char *s)
{
	t_env	*removed;
	t_env	*head;
	t_env	*previous;
	t_env	*next;

	sh->tmp = ft_strjoin(s, "=");
	head = sh->env;
	removed = get_env_side(sh->env, sh->tmp, 0);
	previous = get_env_side(sh->env, sh->tmp, LEFT);
	next = get_env_side(sh->env, sh->tmp, RIGHT);
	free(sh->tmp);
	if (!removed)
		return (1);
	free(removed->name);
	free(removed->value);
	free(removed);
	if (previous->next == NULL)
		sh->env = next;
	else
	{
		sh->env = previous;
		sh->env->next = next;
		sh->env = head;
	}
	return (0);
}

int	get_env_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

char	*get_env(char *s, t_shell *shell)
{
	t_env	*node;
	char	*itoa_n;

	itoa_n = ft_itoa(g_signal.ret);
	ft_bzero(g_signal.str_status, 9);
	ft_strlcpy(g_signal.str_status, itoa_n, 9);
	free(itoa_n);
	if (!ft_strncmp("?", s, 2))
		return (g_signal.str_status);
	shell->tmp = ft_strjoin(s, "=");
	if (!shell->tmp)
		exit(1);
	node = get_env_side(shell->env, shell->tmp, NONE);
	ft_free(shell->tmp);
	if (node)
		return (node->value);
	return ("");
}
char	**get_env_tab(t_env *env)
{
	int		i;
	char	**tab;

	i = get_env_size(env);
	tab = xmalloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env)
	{
		tab[i++] = ft_strjoin(env->name, env->value);
		env = env->next;
	}
	tab[i] = 0;
	return (tab);
}

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

char	*get_env_line(t_env *env, char *env_line)
{
	while (env)
	{
		if (!ft_strcmp(env->name, env_line))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

t_env	*init_lst(char **tab)
{
	int		i;
	t_env	*env;
	t_env	*res;

	env = new_env(NULL, NULL);
	i = -1;
	while (tab[++i])
		add_env(&env, new_env(get_side(LEFT, tab[i]), get_side(RIGHT, tab[i])));
	res = env->next;
	ft_free(env);
	(!(env = get_env_side(res, "SHLVL=", NONE)) && \
	add_env(&res, new_env(ft_strdup("SHLVL="), ft_strdup("1"))));
	(env != NULL && change_value(env, ft_itoa(atoi(env->value) + 1)));
	return (res);
}

void	add_env_plus_equal(t_shell *shell, char *s)
{
	shell->tmp = ft_strjoin(s, "=");
	if (!shell->tmp)
		exit(1);
	add_env(&shell->env, new_env(get_side(LEFT, shell->tmp), \
	get_side(RIGHT, shell->tmp)));
	ft_free(shell->tmp);
}

int	change_env(t_env *env, char *s)
{
	while (env)
	{
		if (!ft_strncmp(s, env->name, ft_strlen(env->name)))
		{
			free(env->value);
			env->value = get_side(RIGHT, s);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

char	*get_path(char *cmd, char *path)
{
	int		i;
	char	**paths;
	char	*cmd_path;
	char	*res;

	if (!path)
		return (0);
	cmd_path = ft_strjoin("/", cmd);
	i = -1;
	paths = ft_split(path, ':');
	res = ft_strjoin(paths[0], cmd_path);
	while (access(res, X_OK) && paths[++i])
	{
		ft_free(res);
		res = ft_strjoin(paths[i], cmd_path);
	}
	if (access(res, X_OK))
		ft_free(res);
	free(cmd_path);
	i = -1;
	while (paths[++i])
		ft_free(paths[i]);
	ft_free(paths);
	return (res);
}
