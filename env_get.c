/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niespana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:29:29 by niespana          #+#    #+#             */
/*   Updated: 2023/04/12 14:29:31 by niespana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		res = ft_free(res);
	ft_free(cmd_path);
	i = -1;
	while (paths[++i])
		ft_free(paths[i]);
	ft_free(paths);
	return (res);
}

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
