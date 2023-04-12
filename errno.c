#include "minishell.h"

int	valid_identifier(char *s)
{
	int	i;
	int	first_occurence;
	int	check;

	check = 0;
	first_occurence = 0;
	i = -1;
	while (s[++i])
	{
		if (!check && first_occurence)
			return (0);
		((s[i] == '=' && first_occurence++) || ((s[i] != '=' && check != '"' \
		&& check != '\'' && check != '?') && check++));
		if (!first_occurence && s[i] == '+' && s[i + 1] == '=')
			return (2);
		if (!first_occurence && !ft_isalnum(s[i]) && s[i] != '_' && \
		!(s[i] == '+' && s[i + 1] == '='))
			return (print_err(3, "minishell: export: '", s, \
			"': not a valid identifier\n") - 1);
	}
	if (!first_occurence)
		return (3);
	return (1);
}

int	print_err(int nb_arg, ...)
{
	va_list	lst;
	int		i;

	i = -1;
	va_start(lst, nb_arg);
	while (++i < nb_arg)
		ft_putstr_fd((char *)va_arg(lst, char *), 2);
	va_end(lst);
	return (1);
}

int	empty_line(char *s)
{
	int	i;

	i = -1;
	while (s[++i])
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
	return (1);
}

void	*ft_free(void *to_free)
{
	if (to_free)
		free(to_free);
	return (NULL);
}

void	*xmalloc(size_t size)
{
	void	*malloced_memory;

	malloced_memory = malloc(size);
	if (!malloced_memory)
		exit(1);
	return (malloced_memory);
}
