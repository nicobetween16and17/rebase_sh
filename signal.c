/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niespana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:31:40 by niespana          #+#    #+#             */
/*   Updated: 2023/04/12 14:31:42 by niespana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	funtion_sigint(pid_t pid)
{
	g_signal.ret = 1;
	if (pid == -1)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
		ft_putstr_fd("\n", 1);
}

void	funtion_sigquit(pid_t pid)
{
	char	*nbr;

	nbr = ft_itoa(g_signal.status);
	if (!nbr)
		return ;
	if (pid == -1)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \b\b", 1);
	}
	else
	{
		ft_putstr_fd("Quit: 3\n", 2);
	}
	free(nbr);
}

void	handle_signal(int signo)
{
	g_signal.pid = waitpid(-1, &g_signal.status, WNOHANG);
	if (signo == SIGINT)
		funtion_sigint(g_signal.pid);
	else if (signo == SIGQUIT)
		funtion_sigquit(g_signal.pid);
}

void	handle_signalhdoc(int signo)
{
	g_signal.pid = waitpid(-1, &g_signal.status, WNOHANG);
	if (signo == SIGINT)
	{
		close(1);
		close(g_signal.fd_hdoc);
		g_signal.heredoc = 0;
	}
}

void	set_signal(int heredoc)
{
	if (!heredoc)
	{
		signal(SIGINT, handle_signal);
		signal(SIGQUIT, handle_signal);
	}
	else
	{
		signal(SIGINT, handle_signalhdoc);
		signal(SIGQUIT, handle_signalhdoc);
	}
}
