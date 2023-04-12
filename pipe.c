/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niespana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:31:22 by niespana          #+#    #+#             */
/*   Updated: 2023/04/12 14:31:23 by niespana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close(int fd)
{
	if (fd >= 0)
		close(fd);
}

void	ft_close_pipe(int fd[2], t_shell *sh)
{
	if (close(fd[0]) == -1 && sh->pipe_error++)
		perror("pipe");
	if (close(fd[1]) == -1 && sh->pipe_error++)
		perror("pipe");
}

/*
 * replace the standard inputs with the copy of the original ones to
 * reset to status qo
 */
void	reset_std(t_shell *sh)
{
	dup2(sh->in, 0);
	dup2(sh->out, 1);
}

/*
 * close the file descriptors
 */
void	close_fds(t_shell *sh)
{
	ft_close(sh->infile);
	ft_close(sh->outfile);
}

/*
 * reset the file descriptors
 */
void	reset_fds(t_shell *sh)
{
	sh->infile = 0;
	sh->outfile = 1;
}
