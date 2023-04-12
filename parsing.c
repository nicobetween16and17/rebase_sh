/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niespana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:31:13 by niespana          #+#    #+#             */
/*   Updated: 2023/04/12 14:31:14 by niespana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_words(char *s)
{
	int	i;

	i = -1;
	while (s[++i])
	{
		if (s[i] != '>' && s[i] != '<' && s[i] != ' ')
			return (1);
	}
	return (0);
}

char	**split_trim_redir(char *s)
{
	char	**split;
	int		i;
	char	**res;
	int		len;

	len = 0;
	i = -1;
	split = split_pipe(s, " \t");
	while (split[++i])
		if (!(str_search(split[i], "><") || (i && \
		str_search(split[i - 1], "><") && !has_words(split[i - 1]))))
			len++;
	res = xmalloc(sizeof(char *) * ++len);
	i = -1;
	len = 0;
	while (split[++i])
		if (!(str_search(split[i], "><") || (i && \
		str_search(split[i - 1], "><") && !has_words(split[i - 1]))))
			res[len++] = ft_strdup(split[i]);
	res[len] = 0;
	free_completed_tab(split);
	return (res);
}

void	remove_quotes(char **s)
{
	int		i;
	int		len;
	char	*res;

	len = 0;
	i = -1;
	free_trim(s, " \t");
	while ((*s)[++i])
	{
		if (!is_btwn_q((*s), i) && ((*s)[i] == '"' || (*s)[i] == '\''))
			len++;
	}
	res = xmalloc(sizeof(char) * (++i - len));
	i = -1;
	len = 0;
	while ((*s)[++i])
	{
		if (!(!is_btwn_q((*s), i) && ((*s)[i] == '"' || (*s)[i] == '\'')))
			res[len++] = (*s)[i];
	}
	res[len] = 0;
	ft_free(*s);
	*s = res;
}

char	**get_cmd_tab(char *s)
{
	char	**cmd;
	int		i;

	i = -1;
	if (!str_search(s, "><"))
		cmd = split_pipe(s, " \t");
	else
		cmd = split_trim_redir(s);
	while (cmd[++i])
		remove_quotes(&cmd[i]);
	if (!cmd[0])
		return (ft_free(cmd));
	return (cmd);
}

void	parsing(t_shell *sh)
{
	int	i;

	i = -1;
	sh->splitted = split_pipe(sh->line, "|");
	while (sh->splitted[++i])
		free_trim(&sh->splitted[i], " ");
}
