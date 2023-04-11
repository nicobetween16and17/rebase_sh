/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niespana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:54:23 by niespana          #+#    #+#             */
/*   Updated: 2022/04/16 13:54:25 by niespana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# define BUFFER_SIZE 1

char	*get_next_line(int fd);
int		does_contain(char *s, char c);
int		gnl_strjoin(char **s1, char const *s2);
int		ft_get_start(char *s);
void	gnl_strdup(char **s1, const char *s2);
char	*gnl_strcpy(char *s, char *s2);

#endif