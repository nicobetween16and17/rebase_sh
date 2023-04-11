#include "minishell.h"

int	is_btwn_q(char *s, int i)
{
	int	sgl;
	int	dbl;
	int	j;

	sgl = 0;
	dbl = 0;
	j = -1;
	while (s && s[++j] && j < i)
	{
		((!sgl && !dbl && s[j] == '\"' && ++dbl) || (!dbl && !sgl \
		&& s[j] == '\'' && ++sgl) || (!sgl && dbl && s[j] == '\"' \
		&& dbl--) || (!dbl && sgl && s[j] == '\'' && sgl--));
	}
	if ((sgl && s[j] == '\'') || (dbl && s[j] == '\"'))
		return (0);
	return (sgl || dbl);
}

int	is_charset(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i] && charset[i] != c)
		i++;
	return (charset[i]);
}

char	*str_search(char *s, char *charset)
{
	int	i;

	i = -1;
	while (s && s[++i])
	{
		if (!is_btwn_q(s, i) && is_charset(s[i], charset))
			return (s + i);
	}
	return (NULL);
}

char	*free_trim(char **s, char *charset)
{
	char	*tmp;

	tmp = ft_strtrim(*s, charset);
	ft_free(*s);
	*s = tmp;
	tmp = NULL;
	return (*s);
}

char	*get_filename(char *s)
{
	char	*filename;
	int		len;
	int		i;

	len = 0;
	i = 0;
	if (!s)
		return (NULL);
	while (s[i] && (s[i] == 32 || s[i] == '>' || s[i] == '<'))
		i++;
	while (s[i + len] && (is_btwn_q(s, i + len) || s[i + len] != 32))
		len++;
	filename = xmalloc(sizeof(char) * ++len);
	len = 0;
	while (s[i] && s[i] != 32)
		filename[len++] = s[i++];
	filename[i] = 0;
	if (filename[0] == '"')
		return (free_trim(&filename, "\""));
	return (free_trim(&filename, "'"));
}

int	has_words(char *s)
{
	int	i;

	i = -1;
	while (s[++i])
	{
		if (s[i] != '>' && s[i] != '<' && s[i] != ' ')
			return (1);
	}
	return (0);
}

char	**split_trim_redir(char *s)
{
	char	**split;
	int		i;
	char	**res;
	int		len;

	len = 0;
	i = -1;
	split = split_pipe(s, ' ');
	while (split[++i])
		if (!(str_search(split[i], "><") || (i  && \
		str_search(split[i - 1], "><") && !has_words(split[i - 1]))))
			(printf("valid command %s\n", split[i]) && len++);
	printf("len = %d\n", len);
	res = xmalloc(sizeof(char *) * ++len);
	i = -1;
	len = 0;
	while (split[++i])
		if (!(str_search(split[i], "><") || (i  && \
		str_search(split[i - 1], "><") && !has_words(split[i - 1]))))
			res[len++] = ft_strdup(split[i]);
	res[len] = 0;
	free_completed_tab(split);
	return (res);
}

char	**get_cmd_tab(char *s)
{
	char	**cmd;
	int		i;

	i = -1;
	if (!str_search(s, "><"))
		cmd = split_pipe(s, ' ');
	else
		cmd = split_trim_redir(s);
	while (cmd[++i])
	{
		if (cmd[i][0] == '\'')
			free_trim(&cmd[i], "' ");
		else if (cmd[i][0] == '"')
			free_trim(&cmd[i], "\" ");
	}
	return (cmd);
}
void	parsing(t_shell *sh)
{
	int	i;

	i = -1;
	sh->splitted = split_pipe(sh->line, '|');
	while (sh->splitted[++i])
		free_trim(&sh->splitted[i], " ");
}