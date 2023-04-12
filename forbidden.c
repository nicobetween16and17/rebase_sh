#include "minishell.h"

int	check_pipe(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '|')
		{
			i++;
			while (s[i] && s[i] == ' ')
				i++;
			if (!s[i])
				return (set_error(257 + print_err(2, STX, "|'\n")));
		}
		i++;
	}
	return (0);
}

int	check_filenames(char *s)
{
	int	i;
	int	res;

	res = 0;
	i = 0;
	while (s[i])
	{
		if (!is_btwn_q(s, i) && (s[i] == '>' || s[i] == '<'))
		{
			while (s[i] && (s[i] == '>' || s[i] == '<'))
				i++;
			while (s[i] && s[i] == ' ')
				i++;
			if (!(s[i] && s[i] != '>' && s[i] != '<' && s[i] != '|'))
			{
				res = 1;
				break ;
			}
		}
		i++;
	}
	if (res)
		set_error(257 + print_err(3, STX, &s[i - 1], "'\n"));
	return (res);
}

int	check_forbidden(char *s)
{
	int	i;
	int	start;
	int	type;

	i = 0;
	while (s[i])
	{
		type = 0;
		if (((++type && s[i] == '>') || (++type && s[i] == '<') || \
		(++type && s[i] == '|')) && !is_btwn_q(s, i))
		{
			start = i;
			while (s[i] && ((s[i] == '>' || s[i] == '<' || s[i] == '|') && \
			!is_btwn_q(s, i)))
			{
				if (type == 1 && s[i] == '<' || type == 2 && s[i] == '>')
					return (set_error(257 + print_err(3, STX, &s[i], "'\n")));
				i++;
			}
			if (i - start > 2 || (i - start > 1 && type == 3))
				return (set_error(257 + print_err(3, STX, &s[i - 1], "'\n")));
		}
		(s[i] && i++);
	}
	return (check_pipe(s) || check_filenames(s));
}
