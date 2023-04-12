#include "minishell.h"

void	exec_no_pipe(t_shell *sh)
{
	if (built_in(sh->crt_cmd, sh))
		return ;
	else if (!sh->cmd_path && print_err(3, "minishell: ", \
	sh->crt_cmd[0], ": command not found\n"))
	{
		g_signal.ret = 127;
		return ;
	}
	sh->pid[sh->n_pid] = fork();
	if (sh->pid[sh->n_pid] < 0)
		return ;
	if (!sh->pid[sh->n_pid++] && execve(sh->cmd_path, sh->crt_cmd, sh->env_tab))
		exit(g_signal.ret);
}

void	child_process(t_shell *sh, int first_pipe, int last_pipe, int new_fd[2])
{
	int	is_bi;

	if (!first_pipe)
	{
		dup2(sh->old_fd[0], 0);
		ft_close_pipe(sh->old_fd, sh);
	}
	if (!last_pipe)
	{
		dup2(new_fd[1], 1);
		ft_close_pipe(new_fd, sh);
	}
	is_bi = built_in(sh->crt_cmd, sh);
	(is_bi || execve(sh->cmd_path, sh->crt_cmd, sh->env_tab));
	if (!is_bi && print_err(3, "minishell: ", \
	sh->crt_cmd[0], ": command not found\n"))
		g_signal.ret = 127;
}

void	exec_pipe(t_shell *sh, int last_pipe, int first_pipe)
{
	int	new_fd[2];

	if (!last_pipe)
		pipe(new_fd);
	sh->pid[sh->n_pid] = fork();
	if (!sh->pid[sh->n_pid++])
	{
		child_process(sh, first_pipe, last_pipe, new_fd);
		exit(g_signal.ret);
	}
	else
	{
		if (!first_pipe)
			ft_close_pipe(sh->old_fd, sh);
		if (!last_pipe)
		{
			sh->old_fd[1] = new_fd[1];
			sh->old_fd[0] = new_fd[0];
		}
	}
}

void	exec_line(t_shell *sh, char *current, int last_pipe, int first_pipe)
{
	sh->crt_cmd = get_cmd_tab(current);
	sh->crt_redir = str_search(current, "><");
	sh->crt_redir_filename = get_filename(sh->crt_redir);
	sh->cmd_path = get_path(sh->crt_cmd[0], get_env_line(sh->env, "PATH="));
	if (sh->crt_cmd && ACCES(sh->crt_cmd[0]))
		sh->cmd_path = ft_strdup(sh->crt_cmd[0]);
	while (sh->crt_redir)
	{
		redirect(sh, get_redir_type(sh->crt_redir), sh->crt_redir_filename);
		if (sh->crt_redir + 1 && sh->crt_redir + 2)
			sh->crt_redir = str_search(sh->crt_redir + 2, "><");
		else
			sh->crt_redir = NULL;
		sh->crt_redir_filename = ft_free(sh->crt_redir_filename);
		sh->crt_redir_filename = get_filename(sh->crt_redir);
	}
	if ((!sh->crt_cmd && !set_error(0)) || (access(sh->cmd_path, X_OK) && \
	set_error(125 + print_err(3, SH, sh->crt_cmd[0], PERM))))
		return ;
	if (last_pipe && first_pipe)
		exec_no_pipe(sh);
	else
		exec_pipe(sh, last_pipe, first_pipe);
	sh->crt_cmd = ft_free(sh->crt_cmd);
}

void	loop_exec(t_shell *sh)
{
	int	i;

	i = -1;
	sh->n_pid = 0;
	sh->pipe_error = 0;
	if (array_size(sh->splitted) > 399)
		perror("pipe");
	while (sh->splitted[++i])
	{
		exec_line(sh, sh->splitted[i], sh->splitted[i + 1] == 0, i == 0);
		sh->crt_redir_filename = ft_free(sh->crt_redir_filename);
		unlink(".heredoc.tmp");
		reset_fds(sh);
		reset_std(sh);
	}
	i = -1;
	while (++i < sh->n_pid)
	{
		waitpid(sh->pid[i], &g_signal.status, 0);
		if (WIFEXITED(g_signal.status))
			g_signal.ret = WEXITSTATUS(g_signal.status);
	}
}
