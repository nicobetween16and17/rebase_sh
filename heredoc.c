#include "minishell.h"

static void	setup_herdoc(char **delimiter, t_shell *sh, int *here_doc_file)
{
	g_signal.heredoc = 1;
	sh->tmp = ft_strjoin(*delimiter, "\n");
	*delimiter = sh->tmp;
	*here_doc_file = open(".heredoc.tmp", O_CREAT | O_RDWR | O_TRUNC, 0777);
}

/*
 * read standard input and put it in a hidden file as long as the line is
 * not uniquely composed of the stop word
 */
int	here_doc(char *delimiter, t_shell *sh)
{
	char	*buffer;
	int		here_doc_file;

	setup_herdoc(&delimiter, sh, &here_doc_file);
	if (here_doc_file < 0)
		return (here_doc_file);
	while (1)
	{
		set_signal(1);
		if (g_signal.heredoc == 0)
			break ;
		ft_putstr_fd("> ", 1);
		buffer = get_next_line(1);
		if (!buffer || ((ft_strlen(buffer) == ft_strlen(delimiter)) \
		&& !ft_strcmp(buffer, delimiter)))
			break ;
		else
			ft_putstr_fd(buffer, here_doc_file);
		ft_free(buffer);
	}
	ft_free(buffer);
	free(sh->tmp);
	set_signal(0);
	g_signal.heredoc = 0;
	reset_std(sh);
	return (here_doc_file);
}
