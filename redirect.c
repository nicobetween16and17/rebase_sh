/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niespana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:31:33 by niespana          #+#    #+#             */
/*   Updated: 2023/04/12 14:31:34 by niespana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	acces(char *s)
{
	return (ACCES(s));
}

int	get_redir_type(char *rd)
{
	int	res;

	if (!ft_strlen(rd))
		return (-1);
	res = HDOC - 1;
	if (++res && !ft_strncmp(rd, "<<", 2))
		;
	else if (++res && !ft_strncmp(rd, "<", 1))
		;
	else if (++res && !ft_strncmp(rd, ">>", 2))
		;
	else if (++res && !ft_strncmp(rd, ">", 1))
		;
	return (res);
}

void	redirect(t_shell *sh, int type, char *f)
{
	sh->infile = 0;
	sh->outfile = 1;
	if (type == HDOC)
	{
		sh->infile = here_doc(f, sh);
		close(sh->infile);
		sh->infile = open(TMP, O_RDONLY);
		if (sh->infile < 0)
			sh->infile = open(TMP, O_CREAT | O_RDONLY | O_TRUNC, 0777);
	}
	if (type == INPUT)
	{
		sh->infile = open(f, O_RDONLY);
		if (sh->infile < 0)
			perror("fd");
	}
	if (type == APPEND)
		sh->outfile = open(f, O_CREAT | O_WRONLY | O_APPEND, 0777);
	else if (type == TRUNC)
		sh->outfile = open(f, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (sh->infile != 0 && dup2(sh->infile, 0) != -1)
		close(sh->infile);
	if (sh->outfile != 1 && dup2(sh->outfile, 1) != -1)
		close(sh->outfile);
}
