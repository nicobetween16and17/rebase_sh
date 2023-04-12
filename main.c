#include "minishell.h"

/*
 * add a space at the end of the string
 */
char	*line_plus_space(t_shell *shell)
{
	char	*line;
	int		i;

	i = -1;
	line = xmalloc((ft_strlen(shell->line) + 2) * sizeof(char *));
	while (shell->line[++i])
		line[i] = shell->line[i];
	line[i++] = 32;
	line[i] = 0;
	free(shell->line);
	shell->line = line;
	return (shell->line);
}

/*
 * intercept the signals, displays the prompt with a crux or a 'v' depending
 * of the succes of the last command line. quit if the user uses ctrl+D
 * add the line in the history, replace the dollar signs, tokenize the line
 * and execute them. free the allocated line at the end.
 */
//TODO checkforbidden crash et redir seule pas d'erreur
void	checkline(t_shell *shell)
{
	set_signal(0);
	shell->line = readline("\033[0;32mminishell> \033[0m");
	if (!shell->line)
		shell->exit = 1;
	else
	{
		if ((is_expandable(shell->line, ft_strlen(shell->line), 1) || \
		check_forbidden(shell->line)) && !ft_free(shell->line))
			return ;
		shell->env_tab = get_env_tab(shell->env);
		add_history(shell->line);
		line_plus_space(shell);
		replace_words(shell, -1, 0);
		if (empty_line(shell->line))
			return ;
		parsing(shell);
		loop_exec(shell);
		free_completed_tab(shell->env_tab);
	}
}

void	init_shell(t_shell *sh, char **env)
{
	sh->infile = 0;
	sh->outfile = 1;
	sh->in = dup(0);
	sh->out = dup(1);
	sh->exit = 0;
	sh->env = init_lst(env);
	g_signal.heredoc = 0;
	g_signal.pid = -1;
	g_signal.status = 0;
}

int	main(int ac, char **av, char **env)
{
	t_shell	minishell;

	(void)ac;
	(void)av;
	init_shell(&minishell, env);
	while (!minishell.exit)
	{
		checkline(&minishell);
	}
	free_env_lst(minishell.env);
	return (g_signal.ret);
}
