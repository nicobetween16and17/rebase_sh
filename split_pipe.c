/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niespana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:31:48 by niespana          #+#    #+#             */
/*   Updated: 2023/04/12 14:31:49 by niespana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_wordlen2(char *s, char *c, int j)
{
	size_t	i;

	i = 0;
	while ((s[j + i] && is_btwn_q(s, j + i)) || (!is_charset(s[j + i], c) && \
	s[j + i]))
		i++;
	return (i + 1);
}

size_t	ft_countwords2(char *s, char *c)
{
	size_t	len;
	int		i;

	i = 0;
	len = 0;
	while (s[i] && s[i] == 32)
		i++;
	while (s[i])
	{
		if (is_btwn_q(s, i) || !is_charset(s[i], c))
		{
			len++;
			while (s[i] && (is_btwn_q(s, i) || !is_charset(s[i], c)))
				i++;
		}
		else
			i++;
	}
	return (len);
}

char	**ft_free_all2(char **split, int start)
{
	while (start >= 0)
		free(split[start--]);
	free(split);
	return (0);
}

char	**split_pipe(char *s, char *c)
{
	char	**split;
	int		i;
	int		k;

	i = -1;
	k = 0;
	if (!s)
		return (0);
	split = xmalloc((ft_countwords2(s, c) + 1) * sizeof(char *));
	while (s[++i])
	{
		if ((!i || (is_charset(s[i - 1], c) && !is_btwn_q(s, i - 1))) && \
		!is_charset(s[i], c))
		{
			split[k] = xmalloc((ft_wordlen2(s, c, i) + 1) * sizeof(char));
			ft_strlcpy(split[k++], s + i, ft_wordlen2(s, c, i));
		}
	}
	split[k] = NULL;
	return (split);
}
