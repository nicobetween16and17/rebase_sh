#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <dirent.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
# include "libft/libft.h"

# define NONE 9
# define REDIR 8
# define CMD 0
# define HDOC 1
# define INPUT 2
# define APPEND 3
# define TRUNC 4
# define PIPE 5
# define RIGHT 6
# define LEFT 7
# define ABS(Value)	Value < 0 ? -Value : Value
# define ACCES(file) (file && file[0] && file[0] == '.' && file[1] == '/') ? \
1 : file && file[0] && file[0] == '/' ? 1 : 0
# define SH "minishell: "
# define PERM ": Permission denied\n"
# define STX "minishell: syntax error near unexpected token: '"
# define TMP ".heredoc.tmp"

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}t_env;

typedef struct s_shell
{
	char	*line;
	char	**env_tab;
	t_env	*env;
	pid_t 	pid[400];
	int		n_pid;
	char	**splitted;
	char	**crt_cmd;
	char	*crt_redir;
	char	*crt_redir_filename;
	char	*cmd_path;
	int		exit;
	char	*tmp;
	int		dir;
	int		pipe_error;
	int		in;
	int		out;
	int		infile;
	int		outfile;
	int		old_fd[2];
}t_shell;

typedef struct s_sig
{
	int		status;
	pid_t	pid;
	char	str_status[10];
	int		ret;
	int		fd_hdoc;
	int		heredoc;
}	t_sig;

t_sig g_signal;

//utils
void	free_completed_tab(char **tab);
int		ft_strisnum(char *s);
void	free_completed_tab(char **tab);
char	*empty_freeable_string(void);
int		ft_strcmp(const char *s1, const char *s2);
int		array_size(char **array);

//signal
void	set_signal(int heredoc);

//parsing
int		is_btwn_q(char *s, int i);
void	parsing(t_shell *sh);
char	**get_cmd_tab(char	*cmd_line);
char	*str_search(char *s, char *charset);
char	*get_filename(char *redir);
char	*free_trim(char **s, char *charset);
int		is_charset(char c, char *charset);

//split_pipe
char	**split_pipe(char *s, char *c);

//$
void	replace_words(t_shell *shell, int i, int j);
int		is_expandable(char *s, int i, int open);

//envs
t_env	*init_lst(char **tab);
char	**get_env_tab(t_env *env);
char	*get_env(char *s, t_shell *shell);
char	*get_env_line(t_env *env, char *env_line);
int		remove_env(t_shell *sh, char *s);
int		add_env(t_env **lst, t_env *new);
t_env	*new_env(char *name, char *value);
char	*get_side(int side, char *s);
void	append_env(t_env *env, char *s, t_shell *shell);
void	add_env_plus_equal(t_shell *shell, char *s);
int		change_env(t_env *env, char *s);
char	*get_path(char *cmd, char *path);
int		env_in_alphabetic_order(t_env *env);
void	free_env_lst(t_env *env);
char	*get_simplified_arg(char *s);
void	append_env(t_env *env, char *s, t_shell *shell);
int		change_value(t_env *env, char *new_value);
t_env	*get_env_side(t_env *env_lst, char *s, int side);
//pipe
void	ft_close(int fd);
void	reset_std(t_shell *sh);
void	close_fds(t_shell *sh);
void	reset_fds(t_shell *sh);
void	ft_close_pipe(int fd[2], t_shell *sh);

//builtins && builtins2
int		ft_exit(char **params, t_shell *shell);
int		ft_export(char **params, t_shell *shell);
int		ft_unset(char **params, t_shell *shell);
int		ft_env(char **params, t_shell *shell);
int		ft_pwd(char **params, t_shell *shell);
int		ft_echo(char **params, t_shell *shell);
int		ft_cd(char **params, t_shell *shell);
int		built_in(char **cmd, t_shell *sh);
t_env	*get_next_smallest(t_env *env, char *last);

//errno
int		print_err(int nb_arg, ...);
int		valid_identifier(char *s);
void	*ft_free(void *to_free);
void	*xmalloc(size_t size);
int		empty_line(char *s);

//exec
int		get_redir_type(char *rd);
void	redirect(t_shell *sh, int type, char *f);
int		built_in(char **cmd, t_shell *sh);
void	loop_exec(t_shell *sh);

//redirect
int		get_redir_type(char *rd);
void	redirect(t_shell *sh, int type, char *f);

//heredoc
int		here_doc(char *delimiter, t_shell *sh);
int		set_error(int error_code);

//forbidden
int		check_forbidden(char *s);

#endif