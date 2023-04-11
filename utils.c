#include "minishell.h"

void	*xmalloc(size_t size)
{
	void	*malloced_memory;

	malloced_memory = malloc(size);
	if (!malloced_memory)
		exit(1);
	return (malloced_memory);
}

int	ft_strisnum(char *s)
{
	int	i;

	i = -1;
	while (s && s[++i])
		if (!ft_isdigit(s[i]))
			return (0);
	return (1);
}

void	ft_free(void *to_free)
{
	printf("%p : %s\n", to_free, to_free);
	if (to_free)
		free(to_free);
	to_free = NULL;
}

void	free_completed_tab(char **tab)
{
	int	i;

	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

char	*empty_freeable_string(void)
{
	char	*str;

	str = xmalloc(1);
	str[0] = 0;
	return (str);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
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

int	array_size(char **array)
{
	int		size;

	size = 0;
	while (array[size])
		size++;
	return (size);
}