/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niespana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:29:21 by niespana          #+#    #+#             */
/*   Updated: 2023/04/12 14:29:22 by niespana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	shell->tmp = ft_free(shell->tmp);
}

int	change_env(t_env *env, char *s)
{
	while (env)
	{
		if (!ft_strncmp(s, env->name, ft_strlen(env->name)))
		{
			env->value = ft_free(env->value);
			env->value = get_side(RIGHT, s);
			return (1);
		}
		env = env->next;
	}
	return (0);
}
