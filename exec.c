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
	printf("%s\n", type == HDOC ? "HODC": type == INPUT ? "INPUT" : \
	type == APPEND ? "APPEND" : type == TRUNC ? "TRUNC" : NULL);
	sh->infile = 0;
	sh->outfile = 1;
	if (type == HDOC)
	{
		sh->infile = here_doc(f, sh);
		close(sh->infile);
		sh->infile = open(".heredoc.tmp", O_RDONLY);
		printf("heredoc.c %d\n", sh->infile);
		if (sh->infile < 0 && printf("error\n"))
			sh->infile = open(".heredoc.tmp", O_CREAT | O_RDONLY | O_TRUNC, 0777);
	}
	if (type == INPUT)
	{
		sh->infile = open(f, O_RDONLY);
		if (sh->infile < 0)
			perror("open");
	}
	if (type == APPEND)
		sh->outfile = open(f, O_CREAT | O_WRONLY | O_APPEND, 0777);
	else if (type == TRUNC)
		sh->outfile = open(f, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	printf("current fds [%d->%d]\n", sh->infile, sh->outfile);
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

void	exec_line(t_shell *sh, char *current, int last_pipe, int first_pipe)
{
	sh->crt_cmd = get_cmd_tab(current);
	ft_printf("cmd: %s\n", sh->crt_cmd[0]);
	sh->crt_redir = str_search(current, "><");
	sh->crt_redir_filename = get_filename(sh->crt_redir);
	while (sh->crt_redir)
	{
		printf("redirecting %s, %s, %d\n", sh->crt_redir, sh->crt_redir_filename, get_redir_type(sh->crt_redir));
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
	if (last_pipe && first_pipe && !built_in(sh->crt_cmd, sh))
	{
		sh->cmd_path = get_path(sh->crt_cmd[0], get_env_line(sh->env, "PATH="));
		sh->pid[sh->n_pid] = fork();
		if (sh->pid[sh->n_pid] < 0)
			return;
		if (!sh->pid[sh->n_pid]) {
			if (sh->infile != 0) {
				dup2(sh->infile, 0);
				close(sh->infile);
			}
			if (sh->outfile != 1) {
				dup2(sh->outfile, 1);
				close(sh->outfile);
			}
			execve(sh->cmd_path, sh->crt_cmd, sh->env_tab);
		}
		else
		{
			ft_free(sh->cmd_path);
			sh->n_pid++;
			sh->pid[sh->n_pid] = 0;
			return;
		}
		exit(g_signal.ret);
	}
	else
	{
		//print_err(1,"complexe cmds\n");
		sh->cmd_path = get_path(sh->crt_cmd[0], get_env_line(sh->env, "PATH="));
		//printf("cmdpath = %s\n", sh->cmd_path);
		if (!last_pipe && !first_pipe)
			pipe(sh->pipe_out);
		sh->pid[sh->n_pid] = fork();
		if (sh->pid[sh->n_pid] < 0)
			return ;
		if (!sh->pid[sh->n_pid])
		{
			if (!first_pipe)
			{
				dup2(sh->pipe_in[0], sh->infile);
				close(sh->pipe_in[0]);
				close(sh->pipe_in[1]);
			}
			if (!last_pipe)
			{
				close(sh->pipe_out[0]);
				dup2(sh->pipe_out[1], sh->outfile);
				//close(sh->pipe_out[1]);
			}
			if (!built_in(sh->crt_cmd, sh) && execve(sh->cmd_path, sh->crt_cmd, sh->env_tab))
				perror("pipe");
			exit(g_signal.ret);
		}
		else
		{
			if (!first_pipe)
			{
				close(sh->pipe_out[0]);
				close(sh->pipe_out[1]);
			}
			if (!last_pipe)
			{
				sh->pipe_out[0] = dup(sh->pipe_in[0]);
				sh->pipe_out[1] = dup(sh->pipe_in[1]);
				close(sh->pipe_in[0]);
				close(sh->pipe_in[1]);
			}
		}
	}
	printf("goes here\n");
	sh->n_pid++;
	sh->pid[sh->n_pid] = 0;
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
		//print_err(3, "line is [",sh->splitted[i],"]\n");
		exec_line(sh, sh->splitted[i], sh->splitted[i + 1] == 0, i == 0);
		unlink(".heredoc.tmp");
		reset_fds(sh);
		reset_std(sh);
		//print_err(1, "cat loves food yeyeye\n");
	}

	i = -1;
	while (sh->pid[++i])
	{
		printf("ping\n");
		waitpid(sh->pid[i], &g_signal.status, 0);
		printf("pong\n");
		if (WIFEXITED(g_signal.status))
			g_signal.ret = WEXITSTATUS(g_signal.status);
	}
}