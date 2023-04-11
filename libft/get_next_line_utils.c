/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niespana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:54:35 by niespana          #+#    #+#             */
/*   Updated: 2022/04/16 13:54:36 by niespana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	gnl_strdup(char **s1, const char *s2)
{
	int		i;
	char	*res;

	i = 0;
	free(*s1);
	while (s2 && s2[i] != '\0')
		i++;
	res = malloc(sizeof(char) * (i + 1));
	i = -1;
	while (s2 && s2[++i])
		res[i] = s2[i];
	res[i] = 0;
	*s1 = res;
}

int	does_contain(char *s, char c)
{
	while (*s != '\0')
	{
		if (*s == c)
			return (1);
		s++;
	}
	return (0);
}

char	*gnl_strcpy(char *s, char *s2)
{
	while (*s2 != '\0')
	{
		*s = *s2;
		s++;
		s2++;
	}
	*s = '\0';
	return (&s[0]);
}

int	gnl_strjoin(char **s1, char const *s2)
{
	char	*join;
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (*s1 && (*s1)[i])
		i++;
	while (s2 && s2[j])
		j++;
	join = malloc((i + j + 1) * sizeof(char));
	i = -1;
	if (!join)
		return (0);
	while (*s1 && (*s1)[++i])
		join[i] = (*s1)[i];
	free(*s1);
	while (s2 && *s2)
		join[i++] = *s2++;
	join[i] = '\0';
	*s1 = join;
	return (1);
}

int	ft_get_start(char *s)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
		{
			while (s[i + j + 1])
			{
				s[j] = s[i + j + 1];
				j++;
			}
			s[j] = '\0';
			return (i + j);
		}
		i++;
	}
	return (i);
}
