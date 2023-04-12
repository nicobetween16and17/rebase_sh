#include "minishell.h"

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
