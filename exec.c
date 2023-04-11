#include "minishell.h"

int		get_redir_type(char *rd)
{
	int	res;

	if (!ft_strlen(rd))
		return (-1);
	res = HDOC - 1;
	((++res && !ft_strncmp(rd, "<<", 2)) || \
	(++res && !ft_strncmp(rd, "<", 1)) || \
	(++res && !ft_strncmp(rd, ">>", 2)) || \
	(++res && !ft_strncmp(rd, ">", 1)));
	return (res);
}

//static int	error_open(char *file)
//{
//	g_signal.ret = 1;
//	ft_putstr_fd("minishell: ", 2);
//	ft_putstr_fd(file, 2);
//	ft_putendl_fd(": No such file or directory", 2);
//	return (1);
//}

void	redirect(t_shell *sh, int type, char *f)
{
	sh->infile = 0;
	sh->outfile = 1;
	if (type == HDOC)
	{
		sh->infile = here_doc(f, sh);
		close(sh->infile);
		sh->infile = open(".heredoc.tmp", O_RDONLY);
		if (sh->infile < 0)
			sh->infile = open(".heredoc.tmp", O_CREAT | O_RDONLY | O_TRUNC, 0777);
	}
	if (type == INPUT)
	{
		printf("file is %s\n", f);
		sh->infile = open(f, O_RDONLY);
		if (sh->infile < 0)
			perror("fd");
	}
	if (type == APPEND)
		sh->outfile = open(f, O_CREAT | O_WRONLY | O_APPEND, 0777);
	else if (type == TRUNC)
		sh->outfile = open(f, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (sh->infile != 0)
	{
		dup2(sh->infile, 0);
		close(sh->infile);
	}
	if (sh->outfile != 1)
	{
		dup2(sh->outfile, 1);
		close(sh->outfile);
	}
}

int	built_in(char **cmd, t_shell *sh)
{
	int	is_built_in;

	is_built_in = 0;
	if (!ft_strncmp(cmd[0], "echo", 5) && ++is_built_in)
		g_signal.ret = ft_echo(cmd, sh);
	else if (!ft_strncmp(cmd[0], "cd", 3) && ++is_built_in)
		g_signal.ret = ft_cd(cmd, sh);
	else if (!ft_strncmp(cmd[0], "pwd", 4) && ++is_built_in)
		g_signal.ret = ft_pwd(cmd, sh);
	else if (!ft_strncmp(cmd[0], "export", 7) && ++is_built_in)
		g_signal.ret = ft_export(cmd, sh);
	else if (!ft_strncmp(cmd[0], "unset", 6) && ++is_built_in)
		g_signal.ret = ft_unset(cmd, sh);
	else if (!ft_strncmp(cmd[0], "env", 4) && ++is_built_in)
		g_signal.ret = ft_env(cmd, sh);
	else if (!ft_strncmp(cmd[0], "exit", 5) && ++is_built_in)
		g_signal.ret = ft_exit(cmd, sh);
	return (is_built_in);
}

void	exec_no_pipe(t_shell *sh)
{
	printf("no pipe\n");
	sh->cmd_path = get_path(sh->crt_cmd[0], get_env_line(sh->env, "PATH="));
	sh->pid[sh->n_pid] = fork();
	if (sh->pid[sh->n_pid] < 0)
		return;
	if (!sh->pid[sh->n_pid++])
		(built_in(sh->crt_cmd, sh) || execve(sh->cmd_path, sh->crt_cmd, sh->env_tab));
	else
	{
		ft_free(sh->cmd_path);
		return;
	}
	exit(g_signal.ret);
}

void	exec_pipe(t_shell *sh, int last_pipe, int first_pipe)
{
	int new_fd[2];

	if (!last_pipe)
		pipe(new_fd);
	sh->cmd_path = get_path(sh->crt_cmd[0], get_env_line(sh->env, "PATH="));
	sh->pid[sh->n_pid] = fork();
	if (!sh->pid[sh->n_pid++])
	{
		if (!first_pipe)
		{
			dup2(sh->old_fd[0], 0);
			close(sh->old_fd[0]);
			close(sh->old_fd[1]);
		}
		if (!last_pipe)
		{
			close(new_fd[0]);
			dup2(new_fd[1], 1);
			close(new_fd[1]);
		}
		(built_in(sh->crt_cmd, sh) || execve(sh->cmd_path, sh->crt_cmd, sh->env_tab));
		exit(g_signal.ret);
	}
	else
	{
		if (!first_pipe)
		{
			close(sh->old_fd[0]);
			close(sh->old_fd[1]);
		}
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
	while (sh->crt_redir)
	{
		redirect(sh, get_redir_type(sh->crt_redir), sh->crt_redir_filename);
		if (sh->crt_redir + 1 && sh->crt_redir + 2)
			sh->crt_redir = str_search(sh->crt_redir + 2, "><");
		else
		{
			ft_free(sh->crt_redir_filename);
			break ;
		}
		ft_free(sh->crt_redir_filename);
		sh->crt_redir_filename = get_filename(sh->crt_redir);
	}
	if (last_pipe && first_pipe)
		exec_no_pipe(sh);
	else
		exec_pipe(sh, last_pipe, first_pipe);
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